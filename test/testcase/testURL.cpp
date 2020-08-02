#include <iostream>
#include <stdlib.h>
#include "utils/url.h"
#include "DebugAssertion.h"
#include "Debug.h"

using namespace std;

bool testURLEncodeAndDecode()
{
	cout << "in function testURLEncode" << endl;

	NacosString originalContent = "Hello W! orld \\/,.%$@^%#*43543";
	NacosString encoded = urlencode(originalContent);
	cout << "Encoded:" << encoded << endl;
	NacosString decoded = urldecode(encoded);
	SHOULD_BE_TRUE(originalContent == decoded, "After encoding and decoding, the content should remain the same.");

	return true;
}
