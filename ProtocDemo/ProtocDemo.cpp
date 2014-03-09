#include "x.pb.h"
using namespace x;

int main()
{
	Person p;
	p.set_name("tom");
	p.set_id(88);
	p.set_email("xx@xx.com");

	std::string str;
	p.SerializeToString(&str);

	printf("%s\n", str.c_str());

	Person x;
	x.ParseFromString(str);
	printf("x.name = %s\n", x.name().c_str());
	printf("x.email= %s\n", x.email().c_str());

	return 0;
}