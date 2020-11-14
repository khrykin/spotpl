//
// Created by Dmitry Khrykin on 03.08.2020.
//

#include <iostream>

// Suppress MSVC warnings from boost
#pragma warning(push, 0)
#pragma warning(disable : 4702)

#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>

#include "request.h"

#pragma warning(pop)

#include "errors.h"


#pragma mark - Making Requests

namespace web {
    namespace net = boost::asio;// from <boost/asio.hpp>
    namespace ssl = net::ssl;   // from <boost/asio/ssl.hpp>

    using tcp = net::ip::tcp;// from <boost/asio/ip/tcp.hpp>


#pragma mark - Regular Expressions

    namespace {
        auto host_regex() {
            static const auto regex = std::regex(R"(:\/\/([^\/]+)\/?)");

            return regex;
        }

        auto target_regex() {
            static const auto regex = std::regex(R"(:\/\/[^\/]+(\/[^\?]+))");

            return regex;
        }
    }

#pragma mark - Construction

    request::request(const std::string &url, web::method method) {
        auto protocol = url.substr(0, url.find("://"));

        std::smatch host_match;
        std::smatch target_match;

        if (!std::regex_search(url, host_match, host_regex()) ||
            !std::regex_search(url, target_match, target_regex()))
            throw error::generic("Invalid URL");

        port = protocol == "https" ? "443" : "80";
        host = host_match[1].str();
        target = target_match[1].str();

        req = http::request<http::string_body>(method, target, 11);
        req.set(http::field::host, host);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        req.set(http::field::content_type, "application/x-www-form-urlencoded");
    }

#pragma mark - Making Request

    void request::make() {
        beast::error_code ec;

        // The io_context is required for all I/O
        net::io_context ioc;

        // The SSL context is required, and holds certificates
        ssl::context ctx(ssl::context::tlsv12_client);

        // Don't Verify the remote server's certificate
        ctx.set_verify_mode(ssl::verify_none);

        // These objects perform our I/O
        tcp::resolver resolver(ioc);
        beast::ssl_stream<beast::tcp_stream> stream(ioc, ctx);

        // Set SNI Hostname (many hosts need this to handshake successfully)
        if (!SSL_set_tlsext_host_name(stream.native_handle(), host.c_str())) {
            ec = beast::error_code{static_cast<int>(::ERR_get_error()), net::error::get_ssl_category()};
        }

        // Look up the domain name
        auto const results = resolver.resolve(host, port);

        // Make the connection on the IP address we get from a lookup
        beast::get_lowest_layer(stream).connect(results);

        // Perform the SSL handshake
        stream.handshake(ssl::stream_base::client);

        // Set content length
        req.content_length(req.body().size());

        // Send the HTTP request to the remote host
        http::write(stream, req);

        // This buffer is used for reading and must be persisted
        beast::flat_buffer buffer;

        // Receive the HTTP response
        http::read(stream, buffer, res);

        //        // Write the message to standard out
        //        std::cout << res << std::endl;

        // Gracefully close the stream
        stream.shutdown(ec);

        if (ec == net::error::eof || ec == ssl::error::stream_truncated) {
            // Rationale:
            // http://stackoverflow.com/questions/25587403/boost-asio-ssl-async-shutdown-always-finishes-with-an-error
            ec = {};
        }

        if (ec) {
            std::cerr << "API Error: " << ec << "\n";
        }
    }

    auto request::body() -> std::string & {
        return req.body();
    }

    auto request::response_text() -> std::string {
        return res.body();
    }
}
