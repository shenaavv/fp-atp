#include <iostream>
#include <fstream>
#include <locale>
#include <string>
#include "spotify.h"

enum State { MAIN = 1, SONG, PLAYLIST, DETAIL_PLAYLIST};
enum MainMenu { MENU_SONG = 1, MENU_PLAYLIST, MENU_EXIT };
enum SongMenu { SONG_BACK = 0, SONG_LIST, SONG_ADD, SONG_DELETE };
enum PlaylistMenu { PLAYLIST_BACK = 0, PLAYLIST_LIST, PLAYLIST_ADD, PLAYLIST_DELETE, PLAYLIST_SELECT };
enum PlaylistDetailMenu { PLAYLIST_DETAIL_BACK = 0, PLAYLIST_DETAIL_LIST, PLAYLIST_DETAIL_ADD, PLAYLIST_DETAIL_DELETE };

// Function prototypes
void printWithRGB(std::string text, int rStart, int gStart, int bStart, int rEnd, int gEnd, int bEnd);
void mainMenu(State &state);
void songMenu(State &state, spotify::Song *&song);
void playlistMenu(State &state, spotify::Playlist *&playlist, spotify::Playlist *&selectedPlaylist);
void playlistDetailMenu(State &state, spotify::Playlist *&playlist, spotify::Playlist *selectedPlaylist);

int main() {
    spotify::Playlist *playlist = nullptr;
    spotify::Playlist *selectedPlaylist = nullptr;
    spotify::Song *song = nullptr;
    spotify::Song* tail = nullptr;
    std::ifstream file("RecomendationSongs.txt"); // Membuka file teks
    std::string line;

    while (getline(file, line)) {
        size_t delimiterPos = line.find(" - ");
        if (delimiterPos != std::string::npos) {
            std::string title = line.substr(0, delimiterPos);
            std::string artist = line.substr(delimiterPos + 3);

            spotify::Song* newSong = new spotify::Song{title, artist, nullptr};

            if (!song) {
                song = newSong;
                tail = newSong;
            } else {
                tail->next = newSong;
                tail = newSong;
            }
         } 
    }

    file.close();

    bool end = false;
    State state = MAIN;
    
    std::ofstream outFile("PlaylistAku.txt", std::ios::out);
    
    printWithRGB("  _____             _   _  __            ", 29, 100, 84, 120, 255, 120);
    printWithRGB(" / ____|           | | (_)/ _|           ", 29, 110, 84, 120, 255, 120); 
    printWithRGB("| (___  _ __   ___ | |_ _| |_ ___ _   _  ", 29, 120, 84, 120, 255, 120);
    printWithRGB(" \\___ \\| '_ \\ / _ \\| __| |  _/ _ \\ | | | ", 29, 130, 84, 120, 255, 120);
    printWithRGB(" ____) | |_) | (_) | |_| | ||  __/ |_| | ", 29, 140, 84, 120, 255, 120);
    printWithRGB("|_____/| .__/ \\___/ \\__|_|_| \\___|\\__, | ", 29, 150, 84, 120, 255, 120);
    printWithRGB("       | |                         __/ | ", 29, 160, 84, 120, 255, 120);
    printWithRGB("       |_|                        |___/  ", 29, 170, 84, 120, 255, 120);

    printWithRGB("          Welcome to Spotifey!           ", 30, 215, 96, 120, 255, 120);
    printWithRGB("    Our Personal Music Playlist Manager  ", 30, 215, 96, 120, 255, 120); 

    while (true) {
        switch (state) {
        case MAIN:
            mainMenu(state);
            break;
        case SONG:
            songMenu(state, song);
            break;
        case PLAYLIST:
            playlistMenu(state, playlist, selectedPlaylist);
            break;
        case DETAIL_PLAYLIST:
            playlistDetailMenu(state, playlist, selectedPlaylist);
            break;
        }
        if (end) break;
    }

    return 0;
}

void printWithRGB(std::string text, int rStart, int gStart, int bStart, int rEnd, int gEnd, int bEnd) {
    int stepR = (rEnd - rStart) / text.length();
    int stepG = (gEnd - gStart) / text.length();
    int stepB = (bEnd - bStart) / text.length();

    for (std::size_t i = 0; i < text.length(); i++) {
        int rValue = rStart + (stepR * i);
        int gValue = gStart + (stepG * i);
        int bValue = bStart + (stepB * i);

        std::cout << "\033[38;2;" << rValue << ";" << gValue << ";" << bValue << "m" << text[i] << "\033[0m";
    }
    std::cout << std::endl;
}

void mainMenu(State &state) {
    int choice;
    std::cout << "\033[38;2;255;255;0m===============================================\033[0m\n";
    std::cout << "\033[38;2;255;182;193m" R"(
|\/| _ . _   |\/| _  _    
|  |(_||| |  |  |(/_| ||_|
    )" "\033[0m\n";
                          

    std::cout << "1. Song\n";
    std::cout << "2. Playlist\n";
    std::cout << "3. Exit\n";
    std::cout << "Choose (1/2/3): ";
    std::cin >> choice;

    switch (choice) {
    case MENU_SONG:
        state = SONG;
        break;
    case MENU_PLAYLIST:
        state = PLAYLIST;
        break;
    case MENU_EXIT:
        std::ofstream outFile("PlaylistAku.txt", std::ios::out);
        if (!outFile) {
            std::cerr << "Error: Unable to open PlaylistAku.txt\n";
            break;
        }
        outFile.close(); // Pastikan file tertutup
        std::cout << "Exiting program...\n";
        std::exit(0);
    }
}

void songMenu(State &state, spotify::Song *&song) {
    int choice;
    std::cout << "\033[38;2;255;255;0m===============================================\033[0m\n";
    std::cout << "\033[38;2;255;182;193m" R"(
 _
(  _  _  _   |\/| _  _    
_)(_)| |(_|  |  |(/_| ||_|
         _|               

)" "\033[0m\n";
    std::cout << "0. Back\n";
    std::cout << "1. List songs\n";
    std::cout << "2. Add song\n";
    std::cout << "3. Delete song\n";
    std::cout << "Choose (0/1/2/3): ";
    std::cin >> choice;

    switch (choice) {
    case SONG_LIST:
        spotify::listSongs(song);
        break;
    case SONG_ADD:
        spotify::addSong(song);
        break;
    case SONG_DELETE:
        spotify::deleteSong(song);
        break;
    case SONG_BACK:
        state = MAIN;
        break;
    default:
        std::cout << "Invalid choice.\n";
        break;
    }
}

void playlistMenu(State &state, spotify::Playlist *&playlist, spotify::Playlist *&selectedPlaylist) {
    int choice;
    bool success = false;
    std::cout << "\033[38;2;255;255;0m===============================================\033[0m\n";
    std::cout << "\033[38;2;255;182;193m" R"(
|_)| _   |. __|_  |\/| _  _    
|  |(_|\/||_\ |   |  |(/_| ||_|
       /                       
)" "\033[0m\n";
    std::cout << "0. Back\n";
    std::cout << "1. List playlists\n";
    std::cout << "2. Add playlist\n";
    std::cout << "3. Delete playlist\n";
    std::cout << "4. Select playlist\n";
    std::cout << "Choose (0/1/2/3/4): ";
    std::cin >> choice;

    switch (choice) {
    case PLAYLIST_LIST:
        spotify::printPlaylist(playlist);
        break;
    case PLAYLIST_ADD:
        spotify::addPlaylist(playlist);
        break;
    case PLAYLIST_DELETE:
        spotify::deletePlaylist(playlist);
        break;
    case PLAYLIST_SELECT:
    {
        spotify::selectPlaylist(playlist, selectedPlaylist, success);
        if (success && selectedPlaylist){
            state = DETAIL_PLAYLIST;
        }
        break;
    }
    case PLAYLIST_BACK:
        state = MAIN;
        break;
    default:
        std::cout << "Invalid choice.\n";
        break;
    }
}

void playlistDetailMenu(State &state, spotify::Playlist *&playlist, spotify::Playlist *selectedPlaylist) {

    int choice;
    std::cout << "\033[38;2;255;255;0m===============================================\033[0m\n";
    std::cout << "\033[38;2;255;182;193m" R"(
|\/|    |_)| _   |. __|_
|  |\/  |  |(_|\/||_\ | 
    /          /        
)" "\033[0m\n";
    std::cout << "Playlist: " << selectedPlaylist->name << std::endl;
    std::cout << "0. Back\n";
    std::cout << "1. List songs\n";
    std::cout << "2. Add song\n";
    std::cout << "3. Delete song\n";
    std::cout << "Choose (0/1/2/3): ";
    std::cin >> choice;

    switch (choice) {
    case PLAYLIST_DETAIL_LIST:
        spotify::listPlaylistSongs(selectedPlaylist);
        break;
    case PLAYLIST_DETAIL_ADD:
        spotify::addSongToPlaylist(playlist, selectedPlaylist);
        break;
    case PLAYLIST_DETAIL_DELETE:
        spotify::deleteSongFromPlaylist(playlist, selectedPlaylist);
        break;
    case PLAYLIST_DETAIL_BACK:
        state = PLAYLIST;
        break;
    default:
        std::cout << "Invalid choice.\n";
        break;
    }
}
