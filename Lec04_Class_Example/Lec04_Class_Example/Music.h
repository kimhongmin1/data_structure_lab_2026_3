#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;

//Music Class
class Music {
private:
	string title;
	string artist;
	string album;
	int year;

public:
	Music(string title, string artist, string album, int year) {
		this->title = title;         // 자신의 멤버 변수에 매개변수 값을 할당
		this->artist = artist;
		this->album = album;
		this->year = year;
	}
	string getTitle() {
		return title;
	}
	string getArtist() {
		return artist;
	}
	string getAlbum() {
		return album;
	}
	int getYear() {
		return year;
	}
};

// Music Streaming Service Class
class MusicStreamingService {
private:
	string serviceName;
	vector<Music> musicList;  // 음악 라이브러리를 벡터로 관리
public:
	MusicStreamingService(string name) {
		this->serviceName = name;
	}
	// Add a music to the service
	void addMusic(string title, string artist, string album, int year) {
		Music newMusic(title, artist, album, year);
		musicList.push_back(newMusic);  // 벡터에 새로운 음악 추가
		cout << title << " by " << artist << " added to " << serviceName << endl;
	}
	// Search for music by title
	Music* searchByTitle(string title) {
		for (int i = 0; i < musicList.size(); i++) {
			if (musicList[i].getTitle() == title) {
				return &musicList[i];  // 음악을 찾으면 해당 음악의 주소 반환 => musicList+i와 동일
			}
		}
		return NULL;  // 음악을 찾지 못하면 NULL 반환
	}
	// Search for music by artist
	vector<Music*> searchByArtist(string artist) {
		vector<Music*> result;  // 검색 결과를 저장할 벡터
		for (int i = 0; i < musicList.size(); i++) {
			// artist와 musicList에 있는 artist가 일치하면 result에 추가
			if (musicList[i].getArtist() == artist) {
				result.push_back(&musicList[i]);  // 음악을 찾으면 해당 음악의 주소를 결과 벡터에 추가
			}
		}
		return result;  // 검색 결과 반환
	}
};