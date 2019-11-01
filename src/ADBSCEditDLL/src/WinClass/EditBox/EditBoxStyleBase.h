
#define STR_(A) # A
#define NONE_

EBSTYLE__(
	TEXTS,              // 0 [id/enumerator]
	236, 236, 236,      // 0 [black theme]
	40, 40, 40,         // 0 [white theme]
	NONE_
)
EBSTYLE__(
	BG,                 // 1
	40, 40, 40,         // 1
	250, 250, 250,      // 1
	NONE_
)
EBSTYLE__(
	CARET,              // 2
	255, 255, 255,      // 2
	60, 60, 60,         // 2
	NONE_
)
EBSTYLE__(
	SYNTAX,             // 3
	255, 255, 192,      // 3
	0, 100, 0,          // 3
	NONE_
)
EBSTYLE__(
	STRING,             // 4
	153, 204, 153,      // 4
	0, 53, 250,         // 4
	NONE_
)
EBSTYLE__(
	CHARS,              // 5
	255, 153, 151,      // 5
	222, 110, 0,        // 5
	NONE_
)
EBSTYLE__(
	NUMBERS,            // 6
	252, 175, 62,       // 6
	235, 106, 0,        // 6
	NONE_
)
EBSTYLE__(
	COMMENT,            // 7
	136, 138, 133,      // 7
	136, 138, 133,      // 7
	NONE_
)
EBSTYLE__(
	IDENT,              // 8
	236, 236, 236,      // 8
	64, 64, 64,         // 8
	NONE_
)
EBSTYLE__(
	LABEL,              // 9
	176, 138, 230,      // 9
	128, 0, 128,        // 9
	NONE_
)
EBSTYLE__(
	PREPROC,            // 10
	255, 142, 107,      // 10
	80, 80, 80,         // 10
	NONE_
)
EBSTYLE__(
	INDENT,             // 11
	90, 90, 90,         // 11
	128, 128, 128,      // 11
	NONE_
)
EBSTYLE__(
	FOLDER,             // 12
	40, 40, 40,         // 12
	148, 148, 148,      // 12
	NONE_
)
EBSTYLE__(
	ACTIVELINE,         // 13
	60, 60, 60,         // 13
	240, 240, 240,      // 13
	NONE_
)
EBSTYLE__(
	FINDIND,            // 14
	0, 134, 0,          // 14
	0, 134, 0,          // 14
	NONE_
)
EBSTYLE__(
	WORD1,              // 15
	56, 130, 221,       // 15
	18, 130, 194,       // 15
	NONE_
)
EBSTYLE__(
	WORD2,              // 16
	114, 159, 207,      // 16
	75, 138, 237,       // 16
	NONE_
)
EBSTYLE__(
	WORD3,              // 17
	152, 204, 102,      // 17
	85, 170, 0,         // 17
	NONE_
)
EBSTYLE__(
	WORD4,              // 18
	176, 138, 230,      // 18
	217, 36, 244,       // 18
	NONE_
)
EBSTYLE__(
	WORD5,              // 19
	90, 188, 188,       // 19
	5, 163, 109,        // 19
	NONE_
)
EBSTYLE__(
	WORD6,              // 20
	90, 160, 190,       // 20
	5, 150, 163,        // 20
	NONE_
)
EBSTYLE__(
	WORD7,              // 21
	60, 140, 140,       // 21
	20, 160, 200,       // 21
	NONE_
)

#undef STR_
#undef NONE_
#if defined(EBSTYLE__)
#   undef EBSTYLE__
#endif
