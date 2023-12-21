#include "cpistr.h"

const char *copypasta1 = "What the procreate did you just fornicating say about me, you little hallion? I'll have you cognize I graduated most elite in my division in the Navy Seals, and I've been enmeshed in multifarious undisclosed incursions on Al-Quaeda, and I have over 300 habituated executions. I am experienced in guerilla campaign and I'm the most qualigied sharpshooter in the full US armed forces. You are nothing to me but just an additional victim. I will obliterate you the copulate out with rigor the likes of which has at no time been seen previously on this macrocosm, indicate my fricking units of language. You assume you can be acquit with saying that fecal matter to me via the Information Superhighway? Acknowledge anew, undesirable person. At the time that we communicate I am influencing my covert organization of operatives crosswise the New World and your Internet Protocol is being pursued immediately so you better bolster for the assault, larval fly. The disturbance that annihilates the deplorable petty existence you refer to as your duration. You're frigging late, child under the age of 18. I can be omnipresent, at any unspecified point in time, and I can annihilate you in surplus seven centum techniques, and that's merely with my unequipped metacarpus. Not exclusively am I extensively qualified for hand-to-hand combat, but I have admittance to the total armory of the United States Marine Corps and I will handle it to its absolute breadth to decimate your wrethed derriere off the surface of the large landmass, you minuscule excrement. If only you could have been aware of what unhallowed comeuppance your smol “ingenious” statement was almost to bear downward upon you, perchance you bequest have refrained from unpleasantly speaking. However, you couldn't, you didn't, and now you're reconciling, you accursed tomfool. I will excrete acrimony all over you and you will inundate in it. You're flipping finito.";

const char *copypasta2 = "The FitnessGram (TM) Pacer Test is a multistage aerobic capacity test that progressively gets more difficult as it continues. The 20 meter pacer test will begin in 30 seconds. Line up at the start. The running speed starts slowly, but gets faster each minute after you hear this signal. [beep] A single lap should be completed each time you hear this sound. [ding] Remember to run in a straight line, and run as long as possible. The second time you fail to complete a lap before the sound, your test is over. The test will begin on the word start. On your mark, get ready, start.";

const char *copypasta3 = "You committed the ultimate cardinal sin, you got personal. You, as a team of professionals trying to make money, got personal. You got personal and decided to insult your playbase, calling us \"ass-hats\" and \"freeloaders\". Not a wise move.  We won't forget this. You've set a new tone for the kind of interaction we'll be having with you. It's a cold one. One where there aren't any illusions about the reality of the situation. Previous notions of \"family\" are dead. We are mere consumers to you, and that is obvious. You have chosen to bring in a new era of hostility and bitterness. Well done. Great PR move";

const char *copypasta4 = "Then we'll all fall together. 3 seasons of Support have hardened me against this. Too often has Soldier complained about me not healing when I was trying to fend off a Winston alone. Too often has Genji tried to YOLO SOLO the enemy team then complained about healing. Too often have I been alone on the objective while my teammates rush after an enemy flank like a terrier after a tennis ball. Too often have we watched our tank provide advancing cover, only to watch our teammates run beyond the safety of the shields and be cut down by the enemy team's focused fire. A support doesn't start spiteful. We start wanting to help, wanting to help our team obtain victory. We try to corral our teammate's base impulses, their desires for glory rather than victory. More often than not we fail, over and over again. Slowly but surely we stop being willing to tolerate this maddening behavior. We stop caring about our teammates. It's not fun anymore, its work. We get fed up. We've been trying to keep them alive, game after game, season after season. Now? We're done. They will look upon us while at critical health. They will ask us over voice what we're doing. They will say to report us in all chat. They will huddle around health packs with some small semblance of hope. And how will we respond? \"Need Healing.\"";


#define LINEBREAK printf("\n------------------------------------------------------------------------------\n");

// Testing macroes
#define TEST_INIT	0
#define TEST_CONCAT	0
#define TEST_STRESS	0
#define TEST_RIGHT	0
#define TEST_MID	0
#define TEST_LCASE	1
#define TEST_UCASE	1

int main(){

	if(TEST_INIT)
	{
		LINEBREAK
		printf("beginning initialization test...\n");
		CpiString *helloWorldStr = NULL;

		printf("initializing string...\n");
		initNewStringObj(&helloWorldStr, "Hello world");

		printf("String length, excluding null terminator is %llu\n", helloWorldStr->length);
		printf("and the string is \"%s\"\n", helloWorldStr->contents);
	}


	if(TEST_CONCAT)
	{
		LINEBREAK
		printf("begining concatenation test...\n");

		CpiString *cp1 = NULL;
		CpiString *cp2 = NULL;
		CpiString *cp3 = NULL;
		CpiString *cp4 = NULL;

		initNewStringObj(&cp1, copypasta1);
		initNewStringObj(&cp2, copypasta2);
		initNewStringObj(&cp3, copypasta3);
		initNewStringObj(&cp4, copypasta4);

		printf("concatenating ALL the copypastas...\n");
		appendStringObjs(&cp1, cp2);
		appendStringObjs(&cp1, cp3);
		appendStringObjs(&cp1, cp4);
	
		printf("new string length: %d\n", getStringLength(cp1));
		printf("concatenated string: %s\n", getString(cp1));
	}


	if(TEST_STRESS)
	{
		LINEBREAK
		printf("beginning appendStringObjs() stress test...\n");

		CpiString *acc = NULL;
		CpiString *cp4 = NULL;
		size_t count = 1;

		initNewStringObj(&acc, "");
		initNewStringObj(&cp4, copypasta4);

		do{
			printf("iteration %llu\n", count);
			printf("accumulator string length: %llu\n", getStringLength(acc));
			printf("concatenated string: \"%s\"\n", getString(acc));
			appendStringObjs(&acc, cp4);
			count++;
		} while(count < 10);
	}


	if(TEST_RIGHT)
	{
		LINEBREAK
		printf("Beginning testing of right()...\n");

		size_t l = 0;
		CpiString *str = NULL; initNewStringObj(&str, "hello world");
		CpiString *modded = NULL;

		modded = right(str, l);
		printf("%llu rightmost character(s) from \"%s\" is \"%s\"\n", l, getString(str), getString(modded));
	}


	if(TEST_MID)
	{
		LINEBREAK
		printf("Beginning testing of mid()...\n");

		size_t x = 5;
	 	size_t y = 1;
		
		CpiString *str = NULL; initNewStringObj(&str, "ABCDEF");
		CpiString *out = NULL;


		// Returns string of length y starting at position x from ThisString
		out = mid(str, x, y);
		printf("%llu character(s) from position %llu from string \"%s\" is \"%s\"\n",\
		                                                                        y, x,\
		                                              getString(str), getString(out));
	}

	if(TEST_UCASE)
	{
		LINEBREAK
		printf("Beginning test of ucase()...\n");

		char s = 'a';
		char o;

		o = ucase(s);
		printf("The uppercase character of \'%c\' is \'%c\'\n", s, o);
	}

	if(TEST_LCASE)
	{
		LINEBREAK
		printf("Beginning test of ucase()...\n");

		char s = 'A';
		char o;

		o = lcase(s);
		printf("The lowercase character of \'%c\' is \'%c\'\n", s, o);
	}

	return 0;
}
