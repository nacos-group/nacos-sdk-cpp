#include <iostream>
#include "utils//UuidUtils.h"

using namespace std;

bool testUUID()
{
	cout << "in function testUUID" << endl;

    cout << "Generating UUID..." << endl;
	for (int i = 0; i < 100; i++)
    {
        cout << "UUID:" << UuidUtils::generateUuid() << endl;
    }
    cout << "test end..." << endl;

	return true;
}
