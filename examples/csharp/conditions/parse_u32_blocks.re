// re2csharp $INPUT -o $OUTPUT -i
using System;
using System.Text;

class Lexer
{
	static uint Lex(byte[] str) {
		int cur = 0;
		int mar = 0;
		ulong result = 0;
		ulong u32Limit = (ulong)UInt32.MaxValue + 1;
	 	void Add(ulong numbase, byte offset) {
			result = result * numbase + (ulong)(str[cur-1] - offset);
			if (result >= u32Limit) {
				Console.WriteLine("overflow");
				result = u32Limit;
			}
		}


	/*!re2c
		re2c:yyfill:enable = 0;
		re2c:define:YYCTYPE   = byte;
		re2c:define:YYPEEK    = "str[cur]";
		re2c:define:YYSKIP    = "cur += 1;";
		re2c:define:YYSHIFT   = "cur += @@{shift};";
		re2c:define:YYBACKUP  = "mar = cur;";
		re2c:define:YYRESTORE = "cur = mar;";

		end = "\x00";

		'0b' / [01]        { goto bin; }
		"0"                { goto oct; }
		""   / [1-9]       { goto dec; }
		'0x' / [0-9a-fA-F] { goto hex; }
		*                  { goto err; }
	*/
bin:
	/*!re2c
		end   { goto end; }
		[01]  { Add(2, (byte)'0'); goto bin; }
		*     { goto err; }
	*/
oct:
	/*!re2c
		end   { goto end; }
		[0-7] { Add(8, (byte)'0'); goto oct; }
		*     { goto err; }
	*/
dec:
	/*!re2c
		end   { goto end; }
		[0-9] { Add(10, (byte)'0'); goto dec; }
		*     { goto err; }
	*/
hex:
	/*!re2c
		end   { goto end; }
		[0-9] { Add(16, (byte)'0');    goto hex; }
		[a-f] { Add(16, (byte)'a'-10); goto hex; }
		[A-F] { Add(16, (byte)'A'-10); goto hex; }
		*     { goto err; }
	*/
end:
	if (result < u32Limit) {
		return (uint)result;
	} else {
		return 0;
	}
err:
	return 0;
}

public static void Main() {
	void Test(uint num, string str, object dmp) {
		var b = Encoding.UTF8.GetBytes(str);
		var n = Lex(b);
		if (n != num) {
			throw new ApplicationException(String.Format("error: {0} is not {1}", n, num));
		}
	}
	Test(1234567890, "1234567890\0", null);
	Test(13, "0b1101\0", null);
	Test(0x7fe, "0x007Fe\0", null);
	Test(420, "0644\0", null);
	Test(0, "9999999999\0", null); //eOverflow)
	Test(0, "123??\0", null); //eSyntax)
}
}