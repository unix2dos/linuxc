#include <stdio.h>

typedef union {
	struct {
		unsigned int one:1;
		unsigned int two:3;
		unsigned int three:10;
		unsigned int four:5;
		unsigned int :2;
		unsigned int five:8;
		unsigned int six:8;
	} bitfield;
	unsigned char byte[8];
} demo_type;

int main(void)
{
	demo_type u = {{ 1, 5, 513, 17, 129, 0x81 }};
	printf("sizeof demo_type = %u\n", sizeof(demo_type));
	printf("values: u=%u,%u,%u,%u,%u,%u\n",
	       u.bitfield.one, u.bitfield.two, u.bitfield.three,
	       u.bitfield.four, u.bitfield.five, u.bitfield.six);
	printf("hex dump of u: %x %x %x %x %x %x %x %x \n",
	       u.byte[0], u.byte[1], u.byte[2], u.byte[3],
	       u.byte[4], u.byte[5], u.byte[6], u.byte[7]);
//查看内存布局
	return 0;
}
