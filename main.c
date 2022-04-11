#include "cpistr.h"

const char *copypasta1 = "What the fuck did you just fucking say about me, you little bitch? I\'ll have you know I graduated top of my class in the Navy Seals, and I\'ve been involved in numerous secret raids on Al-Qaeda, and I have over 300 confirmed kills. I am trained in gorilla warfare and I\'m the top sniper in the entire US armed forces. You are nothing to me but just another target. I will wipe you the fuck out with precision the likes of which has never been seen before on this Earth, mark my fucking words. You think you can get away with saying that shit to me over the Internet? Think again, fucker. As we speak I am contacting my secret network of spies across the USA and your IP is being traced right now so you better prepare for the storm, maggot. The storm that wipes out the pathetic little thing you call your life. You\'re fucking dead, kid. I can be anywhere, anytime, and I can kill you in over seven hundred ways, and that\'s just with my bare hands. Not only am I extensively trained in unarmed combat, but I have access to the entire arsenal of the United States Marine Corps and I will use it to its full extent to wipe your miserable ass off the face of the continent, you little shit. If only you could have known what unholy retribution your little \"clever\" comment was about to bring down upon you, maybe you would have held your fucking tongue. But you couldn\'t, you didn\'t, and now you\'re paying the price, you goddamn idiot. I will shit fury all over you and you will drown in it. You\'re fucking dead, kiddo.";

const char *copypasta2 = "Whawt the fuck did uwu juwst fucking say abouwt me, uwu wittwe bitch? i\'ww have uwu know i gwaduated top of my cwass in the navy seaws, awnd i\'ve bewn invowved in numewous secwet waids own aw-qaeda, awnd i have ovew 300 confiwmed kiwws. I am twained in gowiwwa wawfawe awnd i\'m the top snipew in the entiwe us awmed fowces. Uwu awe nothing tuwu me but juwst anothew tawget. I wiww wipe uwu the fuck out with pwecision the wikes of which has nevew bewn seen befowe own thiws eawth, mawk my fucking wowds. Uwu think uwu cawn get away with saying thawt shit tuwu me ovew the intewnet? think again, fuckew. As we speak i am contacting my secwet netwowk of spies acwoss the usa awnd youw ip iws being twaced wight now so uwu bettew pwepawe fow the stowm, maggot. The stowm thawt wipes out the pathetic wittwe thing uwu caww youw wife. Uwu\'we fucking dead, kid. I cawn be anywhewe, anytime, awnd i cawn kiww uwu in ovew seven hundwed ways, awnd thawt\'s juwst with my bawe hands. Nowt onwy am i extensivewy twained in unawmed combat, but i have access tuwu the entiwe awsenaw of the united states mawine cowps awnd i wiww use iwt tuwu its fuww extent tuwu wipe youw misewabwe ass off the face of the continent, uwu wittwe shit. If onwy uwu couwd have known whawt unhowy wetwibution youw wittwe \"cwevew\" comment was abouwt tuwu bwing down upon uwu, maybe uwu wouwd have hewd youw fucking tongue. But uwu couwdn\'t, uwu didn\'t, awnd now uwu\'we paying the pwice, uwu goddamn idiot. I wiww shit fuwy aww ovew uwu awnd uwu wiww dwown in iwt. Uwu\'we fucking dead, kiddo.";

const char *copypasta3 = "So the other day, I was playing rainbow six siege, and I heard one of my teammates make a callout in the voice chat. It was a real life gamer girl. God, I kid you not, I just stopped playing and pulled my dick out. “fuck, Fuck!” I was yelling in voice chat. I just wanted to hear her voice again. “Please,” I moaned. But she left the lobby. I was crying and covered in my own cum, but I remembered that I could find recent teammates in the ubiplay friends tab. I frantically closed down siege and opened the tab, to find out she had TTV IN HER NAME!!! She was streaming, and only had 100 viewers!!! The competition was low, so I made the first move and donated my months rent to her. I was already about to pre. She read my donation in the chat. God this is the happiest I’ve been in a long time. I did a little research, and found out where she goes to school, but I am a little nervous to talk to her in person, and need support. Any advice before my Uber gets to her middle school?";

const char *copypasta4 = "Ok, this is ABSOLUTE fucking bullshit. I went to take a test in class yesterday, and when I saw some sexy looking quadratics, my boner engaged. When I found the y-intercept of the equation, I couldn't help it!!! I closed my eyes, and I TORE my dick to shreds, using whip like motions and pulled with great force. That was one of the best nuts I ever had, just thinking about it now gets me riled up. Thing is, I nutted all over the kid sitting right next to me, and the teacher got all pissed at me, screaming at me for jacking off on a classmate. I told that bitch to shut the fuck up, and that jacking off is a natural, artistic, and beautiful process. He should BE HAPPY that my semen is all over him, maybe he can learn a lesson or two about the culture and art of jacking off. HOWEVER, the teacher didn't agree with me. She KICKED ME OUT of the classroom, and I didn't even finish taking the test. Not only THAT, but they made me clean up my cum after it already dried out and solidified on the carrpet. THATS TORTURE!! Do you know how hard it is to clean dried cum? You CLEAN cum after its FRESH out of your dick, not an hour after you fucking nutted. This is a fucking OUTRAGE. Do you really expect me to not whip out my cock and jack off when i see a HOT quadratic on a test? Either make the equations less sexy, or LET ME jack off in your classroom, asshole.";

#define LINEBREAK printf("\n------------------------------------------------------------------------------\n\n");

// Testing macroes
#define TEST_INIT	1
#define TEST_CONCAT	1
#define TEST_STRESS	1
#define TEST_RIGHT	1

int main(){

	if(TEST_INIT)
	{
		CpiString *helloWorldStr = NULL;

		printf("initializing string...\n");
		initNewStringObj(&helloWorldStr, "Hello world\n");

		printf("String length, excluding null terminator is %llu\n", helloWorldStr->length);
		printf("and the string is %s\n", helloWorldStr->contents);
	}

	LINEBREAK

	if(TEST_CONCAT)
	{
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
	
		printf("new string length: %d\n", getLength(cp1));
		printf("concatenated string: %s\n", getString(cp1));
	}

	LINEBREAK

	if(TEST_STRESS)
	{
		printf("beginning appendStringObjs() stress test...\n");

		CpiString *acc = NULL;
		CpiString *cp4 = NULL;
		size_t count = 1;

		initNewStringObj(&acc, "");
		initNewStringObj(&cp4, copypasta4);

		do{
			printf("iteration %llu\n", count);
			printf("accumulator string length: %llu\n", getLength(acc));
			printf("concatenated string: \"%s\"\n", getString(acc));
			appendStringObjs(&acc, cp4);
			count++;
		} while(count < 10);
	}

	LINEBREAK

	if(TEST_RIGHT)
	{
		printf("beginning testing of right()...\n");

		size_t l = 5;
		CpiString *str = NULL; initNewStringObj(&str, "ooga booga");
		CpiString *modded = NULL;

		modded = right(str, l);
		printf("%d rightmost characters from \"%s\" is \"%s\"\n", l, getString(str), getString(modded));			}


	return 0;
}
