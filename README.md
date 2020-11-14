# spotpl [![Test](https://github.com/khrykin/spotpl/workflows/Test/badge.svg)](https://github.com/khrykin/spotpl/actions)


A command-line utility that creates .m3u playlist file from Spotify playlist, using present files from the local music library.
Works with iTunes/Music.app and with libraries with similar structure.

## Usage

You call `spotpl` with paths to the local music library, URL of Spotify playlist and, optionally, specify an output path:

```shell script
spotpl -l "/Volumes/My HD/iTunes/iTunes Media/Music/" -p "https://open.spotify.com/playlist/37i9dQZF1DX6ujZpAN0v9r"
```

Example output:
```
spotpl -l "/Volumes/My HD/iTunes/iTunes Media/Music" -o ~/Downloads -p https://open.spotify.com/playlist/37i9dQZF1DXaLI8a395lse
                                    
Using a local music library at:

   "/Volumes/My HD/iTunes/iTunes Media/Music"

Found 39/80 tracks from Spotify playlist:

   Alternative 90s

   https://open.spotify.com/playlist/37i9dQZF1DXaLI8a395lse

The following tracks haven't been found:

   Air - Sexy Boy (album: Moon Safari)
   Arab Strap - The First Big Weekend (album: Ten Years of Tears)
   Ash - Girl From Mars (album: The Best Of Ash (Remastered Version))
   The Auteurs - New French Girlfriend (album: Now I'm A Cowboy)
   Babybird - You're Gorgeous (album: Best of Babybird)
   Beastie Boys - Sure Shot (album: Ill Communication)
   Air - All I Need (feat. Beth Hirsch) (album: Moon Safari)
   The Beloved - Sweet Harmony (album: Single File)
   The Beta Band - Dry The Rain (Remaster) (album: Dry the Rain (Remaster))
   Black Box Recorder - Child Psychology (album: England Made Me)
   Black Grape - Reverend Black Grape (album: It's Great When You're Straight ... Yeah)
   Blind Melon - No Rain (album: Blind Melon)
   Black Box Recorder - The Facts of Life (album: The Facts of Life)
   Black Grape - Kelly's Heroes (album: It's Great When You're Straight ... Yeah)
   Broadcast - Echo's Answer (album: Echo's Answer)
   Cake - The Distance (album: Fashion Nugget)
   The Cardigans - Erase / Rewind (album: Gran Turismo)
   The Breeders - Divine Hammer (album: Last Splash)
   Catatonia - Dead from the Waist Down (album: Equally Cursed And Blessed)
   The Bluetones - Marblehead Johnson (album: The Singles)
   Björk - Hyper-ballad (album: Post)
   The Cardigans - My Favourite Game (album: Gran Turismo)
   Catatonia - Mulder and Scully (album: International Velvet)
   Cornershop - Brimful of Asha (Norman Cook Remix) (Single Version) (album: Brimful of Asha)
   Björk - It's Oh So Quiet (album: Post)
   The Chemical Brothers - Let Forever Be (album: Surrender)
   Cast - Fine Time (album: All Change)
   Catatonia - Road Rage (album: International Velvet)
   Cornershop - Good to Be on the Road Back Home Again (album: When I Was Born for the 7th Time)
   The Chemical Brothers - Hey Boy Hey Girl (album: Surrender)
   Daniel Johnston - Some Things Last A Long Time (album: 1990)
   Dodgy - Good Enough (album: Ace A's + Killer B's)
   The Cure - A Letter To Elise (album: Wish)
   Drugstore - El President (album: The Best of Drugstore)
   Echobelly - King of the Kerb (album: On)
   Daniel Johnston - True Love Will Find You In The End (album: Welcome To My World)
   Eels - Novocaine For The Soul (album: Beautiful Freak)
   Dodgy - Staying Out For The Summer (album: The Collection)
   Cornershop - Sleep on the Left Side (album: When I Was Born For The 7th Time (Expanded))
   Embrace - All You Good Good People (album: The Good Will Out)
   Eels - Susan's House (album: Beautiful Freak)

.m3u playlist has been written to:

   "/Users/khrykin/Downloads/Alternative 90s.m3u"

List of not found tracks has been written to:

   "/Users/khrykin/Downloads/Alternative 90s - Not Found.txt"
```
## License
See [LICENSE](https://github.com/khrykin/spotpl/blob/master/LICENSE.md).
