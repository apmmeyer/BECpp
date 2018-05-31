#ifndef TAG_LIST_H
#define TAG_LIST_H

#include <iostream>
#include <string>
#include <stdint.h>
#include <list>
#include "except.h"

using namespace std;

class tag_element{
	private:
		int id;
		uint8_t entete[4], code[6], checksum[2];
		int count;
	public:
		tag_element(uint8_t key[12]);
		~tag_element();
		int compare(uint8_t key[12]);
		void passed();
		void Affiche();
};

class tag_list{
  private:
	list<tag_element> listeTag;
  public:
	tag_list(){};
	~tag_list(){};
	int getLength();
	void addElement(uint8_t key[12]);
	list<tag_element>::iterator searchElement(uint8_t key[12]);
	void countElement(uint8_t key[12]);
	void print();
};

#endif
