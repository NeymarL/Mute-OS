void io_hlt(void);

void HariMain(void)
{
	int i; /* 変数宣言。iという変数は、32ビットの整数型 */
	char *p; /* pという変数は、BYTE [...]用の番地 */

	char *p = (char *) 0xa0000;

	for (int i = 0; i <= 0xffff; i++) {
		*(p + i) = 0x0f;
	}

	for (;;) {
		io_hlt();
	}
}
