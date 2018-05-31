#ifndef EXCEPT_H
#define EXCEPT_H

#include <iostream>
#include <string>

using namespace std;

class _exception{
  private:
    string errorStr;
  public:
    _exception(string inErr) {
      errorStr = inErr;
    }

	void Error(){
		cout<<errorStr<<endl;
	}
};

#endif
