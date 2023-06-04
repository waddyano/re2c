//re2csharp $INPUT -o $OUTPUT -8 -s -i
using System;
using System.Text;

/*!include:re2c "unicode_categories.re" */

class Lexer
{
	static uint Lex(byte[] str) {
		int cur = 0;
		int mar = 0;
	/*!re2c
		re2c:yyfill:enable = 0;
		re2c:define:YYCTYPE   = byte;
		re2c:define:YYPEEK    = "str[cur]";
		re2c:define:YYSKIP    = "cur += 1;";
		re2c:define:YYBACKUP  = "mar = cur;";
		re2c:define:YYRESTORE = "cur = mar;";

		// Simplified "Unicode Identifier and Pattern Syntax"
		// (see https://unicode.org/reports/tr31)
		id_start    = L | Nl | [$_];
		id_continue = id_start | Mn | Mc | Nd | Pc | [\u200D\u05F3];
		identifier  = id_start id_continue*;

		identifier { return 0; }
		*          { return 1; }
	*/
	}

	public static void Main() {
		if (Lex(Encoding.UTF8.GetBytes("_Ыдентификатор\000")) != 0) {
			throw new ApplicationException("Lex failed");
		}
	}
}
