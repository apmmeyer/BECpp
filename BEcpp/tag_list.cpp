#include "tag_list.h"

using namespace std;

static int idelement = 0;

tag_element::tag_element(uint8_t key[12]){
	int i=0;
	for(i = 0; i < 4; i++)
		entete[i] = key[i];
	for(i = 4; i < 10; i++)
		code[i-4] = key[i];
	for(i = 10; i < 12; i++)
		checksum[i-10] = key[i];
	count = 0;
	idelement ++;
	id = idelement;
}

tag_element::~tag_element(){}

int tag_element::compare(uint8_t key[12]){
	int res = 0;
	int i, j;
	// Comparaison partie entête
	for(i = 0; i < 4; i++){
		for(j = 0; j < 8; j++)
			if(((entete[i]>>j)&0x01) == ((key[i]>>j)&0x01))
				res ++;
	}
	// Comparaison partie code
	for(i = 4; i < 10; i++){
		for(j = 0; j < 8; j++)
			if(((code[i-4]>>j)&0x01) == ((key[i]>>j)&0x01))
				res ++;
	}
	// Comparaison partie checksum
	for(i = 10; i < 12; i++){
		for(j = 0; j < 8; j++)
			if(((checksum[i-10]>>j)&0x01) == ((key[i]>>j)&0x01))
				res ++;
	}
	return res;
}

void tag_element::passed(){
	// traite le passage d'un tag particulier
	count ++;
}

void tag_element::Affiche(){
	// Affiche les informations liées au tag	
	int i=0;
	cout<<"[===] Tag n°"<<id<<" [===]"<<endl;
	cout<<"\t Entête : ";
	for(i = 0; i < 4; i++) cout<<entete[i];
	cout<<endl<<"\t Code : ";
	for(i = 4; i < 10; i++) cout<<code[i-4];
	cout<<endl<<"\t checksum : ";
	for(i = 10; i < 12; i++)	cout<<checksum[i-10];
	cout<<endl;
	cout<<"|===| Passage n°"<<count<<" |===|"<<endl;
}



int tag_list::getLength(){
	// Renvoi le nombre de tag enregistrés	
	int i = 0;
	for(list<tag_element>::iterator it = listeTag.begin(); it != listeTag.end(); ++it)
		i ++;
	return i;
}

void tag_list::addElement(uint8_t key[12]){
	// ajoute un élément en fin de liste
	tag_element newel(key);
	listeTag.push_back(newel);
}

list<tag_element>::iterator tag_list::searchElement(uint8_t key[12]){
	// Trouve l'élément ayant au moins 75% de concordance avec le tag recu (25% d'erreur dûs au défaut de réception)	
	int concordance, concMax(0);
	list<tag_element>::iterator Max = listeTag.begin();
	list<tag_element>::iterator it;
	cout<<"DEBUG = starting search"<<endl;
	cout<<"Searching for ";
	for(int i = 0; i < 12; i++){
		cout<<hex<<key[i];
	}
	cout<<endl;
	if(listeTag.end() != listeTag.begin()){
		for(list<tag_element>::iterator itsE = listeTag.begin(); itsE != listeTag.end(); itsE++){
			itsE->Affiche();
			concordance = itsE->compare(key);
			if(concordance > concMax){
				Max = itsE;
				concMax = concordance;
			}
			cout<<"Concordance du tag : "<<dec<<concordance<<" (Max : "<<concMax<<") sur 96"<<endl;
		}
		if(concMax < 87)
			Max = listeTag.end();
	} else
		Max = listeTag.end();
	return Max;
}

void tag_list::countElement(uint8_t key[12]){
	// traite le passage d'un tag en cherchant le tag parmi la liste et affiche les informations du tag passé
	list<tag_element>::iterator Elem = searchElement(key);
	if(Elem == listeTag.end())
		throw _exception("tag_list.cpp: : void tag_list::countElement(uint8_t key[12]) : Element not found.");
	else {
		Elem->passed();
		Elem->Affiche();
	}
}

void tag_list::print(){
	if(listeTag.begin() == listeTag.end()){
		cout<<"liste vide"<<endl;
	} else
		for(list<tag_element>::iterator it = listeTag.begin(); it != listeTag.end(); ++it)
			it->Affiche();
}


