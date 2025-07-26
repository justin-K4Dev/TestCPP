#include "stdafx.h"
#include <Windows.h>

#include <ctime>
#include <sstream>

namespace Time
{
	void printTimeByMacro()
	{
		const char* buildString = "This build was compiled at " __DATE__ ", " __TIME__ ".";

		std::cout << "build time : " << buildString << std::endl;
		std::cout << "local time : " << __TIMESTAMP__ << std::endl;
		/*
		output:
			build time : This build was compiled at Jun 24 2018, 16:46:48.
			local time : Sun Jun 24 16:46:48 2018
		*/

		system("pause");
	}

	std::string getTimeStamp()
	{
		time_t now = time(nullptr);
		tm* gmt_time = gmtime(&now);

		std::ostringstream oss;
		oss << std::put_time(gmt_time, "%Y-%m-%d %H:%M:%S");

		return oss.str();
	}

	void utc_time()
	{
		std::cout << "UTC : " << getTimeStamp() << std::endl;
		/*
		output:
			UTC : 2018-06-24 05:26:30
		*/

		system("pause");
	}

	void utc_time_2_time_t()
	{
		/*
			struct tm
			{
				int tm_sec;		//seconds after the minute - [0, 59]
				int tm_min;		//minutes after the hour - [0, 59]
				int tm_hour;	//hours since midnight - [0, 23]

				int tm_mday;	//day of the month - [1, 31]
				int tm_mon;		//months since January - [0, 11]
				int tm_year;	//years since 1900

				int tm_wday;	//days since Sunday - [0, 6]
				int tm_yday;	//days since January 1 - [0, 365]
				int tm_isdst;	//daylight savings time flag
			};	
		*/

		//current date/time based on current system
		std::time_t now = std::time(nullptr);

		std::cout << "sec since epoch: "	<< now << std::endl;
		std::cout << "year : "				<< std::localtime(&now)->tm_year + 1900		<< std::endl;
		std::cout << "month : "				<< std::localtime(&now)->tm_mon + 1			<< std::endl;
		std::cout << "day : "				<< std::localtime(&now)->tm_mday << "\n"	<< std::endl;
		std::cout << "hour : "				<< std::localtime(&now)->tm_hour			<< std::endl;
		std::cout << "minute : "			<< std::localtime(&now)->tm_min				<< std::endl;
		std::cout << "sec : "				<< std::localtime(&now)->tm_sec				<< std::endl;

		std::cout << "day of the week, month, day, h:m:s year : " << std::asctime(std::localtime(&now)) << std::endl;

		/*
		output:
			sec since epoch: 1529727716
			year : 2018
			month : 6
			day : 23

			hour : 13
			minute : 21
			sec : 56
			day of the week, month, day, h:m:s year : Sat Jun 23 13:21:56 2018
		*/

		system("pause");
	}

	void utc_tm_2_time_t()
	{
		//current date/time based on current system
		time_t now = std::time(nullptr);

		std::tm currentDate;

		currentDate.tm_year = std::localtime(&now)->tm_year;	//years since 1900
		currentDate.tm_mon = std::localtime(&now)->tm_mon;		//months since January - [0, 11]
		currentDate.tm_mday = std::localtime(&now)->tm_mday;	//day of the month - [1, 31]
		currentDate.tm_hour = 0;								//hours since midnight - [0, 23]
		currentDate.tm_min = 0;									//minutes after the hour - [0, 59]
		currentDate.tm_sec = 0;									//seconds after the minute - [0, 59]

		//00:00:00 on the current date
		std::time_t currentDay = std::mktime(&currentDate);	//convert tm -> time_t

		std::cout << "seconds elapsed since 00:00 today : " << std::difftime(now, currentDay) << std::endl;
		/*
		output:
			seconds elapsed since 00:00 today : 71703
		*/

		system("pause");
	}

	time_t mkgmtime(struct tm* tm)
	{
#if defined(_WIN32)
		return _mkgmtime(tm); // for visual c++
#elif defined(linux)
		return timegm(tm);
#endif
	}

	void custom_tm_2_time_t()
	{
		std::tm customDate;

		//set to a custom time
		customDate.tm_year = 2015 - 1900;	//years since 1900
		customDate.tm_mon = 3 - 1;			//months since January - [0, 11]
		customDate.tm_mday = 17;			//day of the month - [1, 31]
		customDate.tm_hour = 13;			//hours since midnight - [0, 23]
		customDate.tm_min = 35;				//minutes after the hour - [0, 59]
		customDate.tm_sec = 25;				//seconds after the minute - [0, 59]

		std::time_t customDay = mkgmtime(&customDate); //convert tm -> time_t

		std::cout << "day of the week, month, day, h:m:s year : " << std::asctime(std::localtime(&customDay)) << std::endl;

		/*
		output:
			day of the week, month, day, h:m:s year : Tue Mar 17 13:35:25 2015
		*/

		system("pause");
	}

	void utc_2_localtime()
	{
		//current date/time based on current system
		time_t now = std::time(nullptr);

		//convert now to tm struct for local timezone
		tm* localtm = std::localtime(&now);
		std::cout << "The local date and time is : " << std::asctime(localtm) << std::endl;

		//Converts given time since epoch as std::time_t value into calendar time,
		//expressed in Coordinated Universal Time (UTC).
		//convert now to tm struct for UTC
		tm* gmtm = std::gmtime(&now);
		if (gmtm != NULL) {
			std::cout << "The UTC date and time is : " << std::asctime(gmtm) << std::endl;
		}
		else {
			std::cerr << "Failed to get the UTC date and time" << std::endl;
			return;
		}

		char buf[512];
		
		//Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
		//for more information about date/time format
		std::strftime(buf, sizeof(buf), "%Y-%m-%d.%X", gmtm);

		//print current date/time, format is YYYY-MM-DD.HH:mm:ss
		std::cout << "UTC : " << buf << std::endl;

		/*
		output:
			The local date and time is : Sat Jun 23 20:03:09 2018
			The UTC date and time is : Sat Jun 23 11:03:09 2018
			UTC : 2018-06-23.11:03:09
		*/

		system("pause");
	}

	void utc_localtime_offset()
	{
		std::time_t now = std::time(nullptr);

		std::time_t local = std::mktime(std::localtime(&now));
		std::time_t utc = std::mktime(std::gmtime(&now));

		long timezone = static_cast<long> (utc - local);

		std::cout << "time zone offset : " << timezone << std::endl;
		/*
		output:
			time zone offset : -32400
		*/
	}

	#define MST (-7)
	#define UTC (0)
	#define CCT (+8)
	#define KST (+9)

	void major_country_utc()
	{
		time_t rawtime;
		tm* ptm;

		std::time(&rawtime);
		ptm = std::gmtime(&rawtime);

		puts("Current time around the World:");

		printf("Phoenix, AZ (U.S.) :  %2d:%02d\n", (ptm->tm_hour + MST) % 24, ptm->tm_min);
		printf("Reykjavik (Iceland) : %2d:%02d\n", (ptm->tm_hour + UTC) % 24, ptm->tm_min);
		printf("Beijing (China) :     %2d:%02d\n", (ptm->tm_hour + CCT) % 24, ptm->tm_min);
		printf("Seoul (Korea) :       %2d:%02d\n", (ptm->tm_hour + KST) % 24, ptm->tm_min);

		/*
		output:
			Current time around the World:
			Phoenix, AZ (U.S.) :  11:09
			Reykjavik (Iceland) : 18:09
			Beijing (China) :      2:09
			Seoul (Korea) :        3:09
		*/

		system("pause");
	}

	void make_local_time_from_utc()
	{
		for(int i = 0; i < 5; ++i) {

			//returns seconds since midnight on January 1, 1970
			int totalSeconds = time(nullptr);
			//calculate the seconds of the current time(second)
			int currentSecond = totalSeconds % 60;
			//calculate total minutes
			int totalMinutes = totalSeconds / 60;
			//minute calculation of current time(minutes)
			int currentMinute = totalMinutes % 60;

			//total time(hours) calculation
			//UTC -> KST Add 9 to change
			int totalHours = totalMinutes / 60 + KST;
			//calculate current time(hours)
			int currentHour = totalHours % 24;

			//print
			std::cout << "Time " << currentHour << ":" << currentMinute << ":" << currentSecond << std::endl;

			Sleep(1000);
		}
		/*
		output:
			Time 3:10:57
			Time 3:10:58
			Time 3:10:59
			Time 3:11:0
			Time 3:11:1	
		*/

		system("pause");
	}


	int setenv(const char *name, const char *value, int overwrite)
	{
		int errcode = 0;
		if (!overwrite) {
			size_t envsize = 0;
			errcode = getenv_s(&envsize, NULL, 0, name);
			if (errcode || envsize)
				return errcode;
		}
		return _putenv_s(name, value);
	}

	#define     DESTZONE    "TZ=Europe/Stockholm"       // Our destination time zone

	void time_zone_change()
	{
		/*
			Time Zone							Country 	Offset		DST	Offset
												Code		from UTC	from UTC
			Africa/Abidjan						CI			+00:00		+00:00
			Africa/Accra						GH			+00:00		+00:00
			Africa/Addis_Ababa					ET			+03:00		+03:00
			Africa/Algiers						DZ			+01:00		+01:00
			Africa/Asmara						ER			+03:00		+03:00
			Africa/Asmera									+03:00		+03:00
			Africa/Bamako						ML			+00:00		+00:00
			Africa/Bangui						CF			+01:00		+01:00
			Africa/Banjul						GM			+00:00		+00:00
			Africa/Bissau						GW			+00:00		+00:00
			Africa/Blantyre						MW			+02:00		+02:00
			Africa/Brazzaville					CG			+01:00		+01:00
			Africa/Bujumbura					BI			+02:00		+02:00
			Africa/Cairo						EG			+02:00		+02:00
			Africa/Casablanca					MA			+00:00		+01:00
			Africa/Ceuta						ES			+01:00		+02:00
			Africa/Conakry						GN			+00:00		+00:00
			Africa/Dakar						SN			+00:00		+00:00
			Africa/Dar_es_Salaam				TZ			+03:00		+03:00
			Africa/Djibouti						DJ			+03:00		+03:00
			Africa/Douala						CM			+01:00		+01:00
			Africa/El_Aaiun						EH			+00:00		+00:00
			Africa/Freetown						SL			+00:00		+00:00
			Africa/Gaborone						BW			+02:00		+02:00
			Africa/Harare						ZW			+02:00		+02:00
			Africa/Johannesburg					ZA			+02:00		+02:00
			Africa/Juba	SS									+03:00		+03:00
			Africa/Kampala						UG			+03:00		+03:00
			Africa/Khartoum						SD			+03:00		+03:00
			Africa/Kigali						RW			+02:00		+02:00
			Africa/Kinshasa						CD			+01:00		+01:00
			Africa/Lagos						NG			+01:00		+01:00
			Africa/Libreville					GA			+01:00		+01:00
			Africa/Lome							TG			+00:00		+00:00
			Africa/Luanda						AO			+01:00		+01:00
			Africa/Lubumbashi					CD			+02:00		+02:00
			Africa/Lusaka						ZM			+02:00		+02:00
			Africa/Malabo						GQ			+01:00		+01:00
			Africa/Maputo						MZ			+02:00		+02:00
			Africa/Maseru						LS			+02:00		+02:00
			Africa/Mbabane						SZ			+02:00		+02:00
			Africa/Mogadishu					SO			+03:00		+03:00
			Africa/Monrovia						LR			+00:00		+00:00
			Africa/Nairobi						KE			+03:00		+03:00
			Africa/Ndjamena						TD			+01:00		+01:00
			Africa/Niamey						NE			+01:00		+01:00
			Africa/Nouakchott					MR			+00:00		+00:00
			Africa/Ouagadougou					BF			+00:00		+00:00
			Africa/Porto-Novo					BJ			+01:00		+01:00
			Africa/Sao_Tome						ST			+00:00		+00:00
			Africa/Timbuktu									+00:00		+00:00
			Africa/Tripoli						LY			+02:00		+02:00
			Africa/Tunis						TN			+01:00		+01:00
			Africa/Windhoek						NA			+01:00		+02:00
			AKST9AKDT										-09:00		-08:00
			America/Adak						US			-10:00		-09:00
			America/Anchorage					US			-09:00		-08:00
			America/Anguilla					AI			-04:00		-04:00
			America/Antigua	AG								-04:00		-04:00
			America/Araguaina					BR			-03:00		-03:00
			America/Argentina/Buenos_Aires		AR			-03:00		-03:00
			America/Argentina/Catamarca			AR			-03:00		-03:00
			America/Argentina/ComodRivadavia				-03:00		-03:00
			America/Argentina/Cordoba			AR			-03:00		-03:00
			America/Argentina/Jujuy				AR			-03:00		-03:00
			America/Argentina/La_Rioja			AR			-03:00		-03:00
			America/Argentina/Mendoza			AR			-03:00		-03:00
			America/Argentina/Rio_Gallegos		AR			-03:00		-03:00
			America/Argentina/Salta				AR			-03:00		-03:00
			America/Argentina/San_Juan			AR			-03:00		-03:00
			America/Argentina/San_Luis			AR			-03:00		-03:00
			America/Argentina/Tucuman			AR			-03:00		-03:00
			America/Argentina/Ushuaia			AR			-03:00		-03:00
			America/Aruba						AW			-04:00		-04:00
			America/Asuncion					PY			-04:00		-03:00
			America/Atikokan					CA			-05:00		-05:00
			America/Atka									-10:00		-09:00
			America/Bahia						BR			-03:00		-03:00
			America/Bahia_Banderas				MX			-06:00		-05:00
			America/Barbados					BB			-04:00		-04:00
			America/Belem						BR			-03:00		-03:00
			America/Belize						BZ			-06:00		-06:00
			America/Blanc-Sablon				CA			-04:00		-04:00
			America/Boa_Vista					BR			-04:00		-04:00
			America/Bogota						CO			-05:00		-05:00
			America/Boise						US			-07:00		-06:00
			America/Buenos_Aires							-03:00		-03:00
			America/Cambridge_Bay				CA			-07:00		-06:00
			America/Campo_Grande				BR			-04:00		-03:00
			America/Cancun						MX			-06:00		-05:00
			America/Caracas						VE			-04:30		-04:30
			America/Catamarca								-03:00		-03:00
			America/Cayenne						GF			-03:00		-03:00
			America/Cayman						KY			-05:00		-05:00
			America/Chicago						US			-06:00		-05:00
			America/Chihuahua					MX			-07:00		-06:00
			America/Coral_Harbour							-05:00		-05:00
			America/Cordoba									-03:00		-03:00
			America/Costa_Rica					CR			-06:00		-06:00
			America/Creston						CA			-07:00		-07:00
			America/Cuiaba						BR			-04:00		-03:00
			America/Curacao						CW			-04:00		-04:00
			America/Danmarkshavn				GL			+00:00		+00:00
			America/Dawson						CA			-08:00		-07:00
			America/Dawson_Creek				CA			-07:00		-07:00
			America/Denver						US			-07:00		-06:00
			America/Detroit						US			-05:00		-04:00
			America/Dominica					DM			-04:00		-04:00
			America/Edmonton					CA			-07:00		-06:00
			America/Eirunepe					BR			-04:00		-04:00
			America/El_Salvador					SV			-06:00		-06:00
			America/Ensenada								-08:00		-07:00
			America/Fort_Wayne								-05:00		-04:00
			America/Fortaleza					BR			-03:00		-03:00
			America/Glace_Bay					CA			-04:00		-03:00
			America/Godthab	GL								-03:00		-02:00
			America/Goose_Bay					CA			-04:00		-03:00
			America/Grand_Turk					TC			-05:00		-04:00
			America/Grenada	GD								-04:00		-04:00
			America/Guadeloupe					GP			-04:00		-04:00
			America/Guatemala					GT			-06:00		-06:00
			America/Guayaquil					EC			-05:00		-05:00
			America/Guyana						GY			-04:00		-04:00
			America/Halifax						CA			-04:00		-03:00
			America/Havana						CU			-05:00		-04:00
			America/Hermosillo					MX			-07:00		-07:00
			America/Indiana/Indianapolis		US			-05:00		-04:00
			America/Indiana/Knox				US			-06:00		-05:00
			America/Indiana/Marengo				US			-05:00		-04:00
			America/Indiana/Petersburg			US			-05:00		-04:00
			America/Indiana/Tell_City			US			-06:00		-05:00
			America/Indiana/Vevay				US			-05:00		-04:00
			America/Indiana/Vincennes			US			-05:00		-04:00
			America/Indiana/Winamac				US			-05:00		-04:00
			America/Indianapolis							-05:00		-04:00
			America/Inuvik						CA			-07:00		-06:00
			America/Iqaluit						CA			-05:00		-04:00
			America/Jamaica						JM			-05:00		-05:00
			America/Jujuy									-03:00		-03:00
			America/Juneau						US			-09:00		-08:00
			America/Kentucky/Louisville			US			-05:00		-04:00
			America/Kentucky/Monticello			US			-05:00		-04:00
			America/Knox_IN									-06:00		-05:00
			America/Kralendijk					BQ			-04:00		-04:00
			America/La_Paz						BO			-04:00		-04:00
			America/Lima						PE			-05:00		-05:00
			America/Los_Angeles					US			-08:00		-07:00
			America/Louisville								-05:00		-04:00
			America/Lower_Princes				SX			-04:00		-04:00
			America/Maceio						BR			-03:00		-03:00
			America/Managua						NI			-06:00		-06:00
			America/Manaus						BR			-04:00		-04:00
			America/Marigot						MF			-04:00		-04:00
			America/Martinique					MQ			-04:00		-04:00
			America/Matamoros					MX			-06:00		-05:00
			America/Mazatlan					MX			-07:00		-06:00
			America/Mendoza									-03:00		-03:00
			America/Menominee					US			-06:00		-05:00
			America/Merida	MX								-06:00		-05:00
			America/Metlakatla					US			-08:00		-08:00
			America/Mexico_City					MX			-06:00		-05:00
			America/Miquelon					PM			-03:00		-02:00
			America/Moncton						CA			-04:00		-03:00
			America/Monterrey					MX			-06:00		-05:00
			America/Montevideo					UY			-03:00		-02:00
			America/Montreal					CA			-05:00		-04:00
			America/Montserrat					MS			-04:00		-04:00
			America/Nassau						BS			-05:00		-04:00
			America/New_York					US			-05:00		-04:00
			America/Nipigon						CA			-05:00		-04:00
			America/Nome						US			-09:00		-08:00
			America/Noronha						BR			-02:00		-02:00
			America/North_Dakota/Beulah			US			-06:00		-05:00
			America/North_Dakota/Center			US			-06:00		-05:00
			America/North_Dakota/New_Salem		US			-06:00		-05:00
			America/Ojinaga						MX			-07:00		-06:00
			America/Panama						PA			-05:00		-05:00
			America/Pangnirtung					CA			-05:00		-04:00
			America/Paramaribo					SR			-03:00		-03:00
			America/Phoenix						US			-07:00		-07:00
			America/Port_of_Spain				TT			-04:00		-04:00
			America/Port-au-Prince				HT			-05:00		-04:00
			America/Porto_Acre								-04:00		-04:00
			America/Porto_Velho					BR			-04:00		-04:00
			America/Puerto_Rico					PR			-04:00		-04:00
			America/Rainy_River					CA			-06:00		-05:00
			America/Rankin_Inlet				CA			-06:00		-05:00
			America/Recife						BR			-03:00		-03:00
			America/Regina						CA			-06:00		-06:00
			America/Resolute					CA			-06:00		-05:00
			America/Rio_Branco					BR			-04:00		-04:00
			America/Rosario									-03:00		-03:00
			America/Santa_Isabel				MX			-08:00		-07:00
			America/Santarem					BR			-03:00		-03:00
			America/Santiago					CL			-04:00		-03:00
			America/Santo_Domingo				DO			-04:00		-04:00
			America/Sao_Paulo					BR			-03:00		-02:00
			America/Scoresbysund				GL			-01:00		+00:00
			America/Shiprock					US			-07:00		-06:00
			America/Sitka						US			-09:00		-08:00
			America/St_Barthelemy				BL			-04:00		-04:00
			America/St_Johns					CA			-03:30		-02:30
			America/St_Kitts					KN			-04:00		-04:00
			America/St_Lucia					LC			-04:00		-04:00
			America/St_Thomas					VI			-04:00		-04:00
			America/St_Vincent					VC			-04:00		-04:00
			America/Swift_Current				CA			-06:00		-06:00
			America/Tegucigalpa					HN			-06:00		-06:00
			America/Thule						GL			-04:00		-03:00
			America/Thunder_Bay					CA			-05:00		-04:00
			America/Tijuana						MX			-08:00		-07:00
			America/Toronto						CA			-05:00		-04:00
			America/Tortola						VG			-04:00		-04:00
			America/Vancouver					CA			-08:00		-07:00
			America/Virgin									-04:00		-04:00
			America/Whitehorse					CA			-08:00		-07:00
			America/Winnipeg					CA			-06:00		-05:00
			America/Yakutat						US			-09:00		-08:00
			America/Yellowknife					CA			-07:00		-06:00
			Antarctica/Casey					AQ			+11:00		+08:00
			Antarctica/Davis					AQ			+05:00		+07:00
			Antarctica/DumontDUrville			AQ			+10:00		+10:00
			Antarctica/Macquarie				AQ			+11:00		+11:00
			Antarctica/Mawson					AQ			+05:00		+05:00
			Antarctica/McMurdo					AQ			+12:00		+13:00
			Antarctica/Palmer					AQ			-04:00		-03:00
			Antarctica/Rothera					AQ			-03:00		-03:00
			Antarctica/South_Pole				AQ			+12:00		+13:00
			Antarctica/Syowa					AQ			+03:00		+03:00
			Antarctica/Vostok					AQ			+06:00		+06:00
			Arctic/Longyearbyen					SJ			+01:00		+02:00
			Asia/Aden							YE			+03:00		+03:00
			Asia/Almaty							KZ			+06:00		+06:00
			Asia/Amman							JO			+02:00		+03:00
			Asia/Anadyr							RU			+12:00		+12:00
			Asia/Aqtau							KZ			+05:00		+05:00
			Asia/Aqtobe							KZ			+05:00		+05:00
			Asia/Ashgabat						TM			+05:00		+05:00
			Asia/Ashkhabad									+05:00		+05:00
			Asia/Baghdad						IQ			+03:00		+03:00
			Asia/Bahrain						BH			+03:00		+03:00
			Asia/Baku							AZ			+04:00		+05:00
			Asia/Bangkok						TH			+07:00		+07:00
			Asia/Beirut							LB			+02:00		+03:00
			Asia/Bishkek						KG			+06:00		+06:00
			Asia/Brunei							BN			+08:00		+08:00
			Asia/Calcutta									+05:30		+05:30
			Asia/Choibalsan						MN			+08:00		+08:00
			Asia/Chongqing						CN			+08:00		+08:00
			Asia/Chungking									+08:00		+08:00
			Asia/Colombo						LK			+05:30		+05:30
			Asia/Dacca										+06:00		+06:00
			Asia/Damascus						SY			+02:00		+03:00
			Asia/Dhaka							BD			+06:00		+06:00
			Asia/Dili							TL			+09:00		+09:00
			Asia/Dubai							AE			+04:00		+04:00
			Asia/Dushanbe						TJ			+05:00		+05:00
			Asia/Gaza							PS			+02:00		+03:00
			Asia/Harbin							CN			+08:00		+08:00
			Asia/Hebron							PS			+02:00		+03:00
			Asia/Ho_Chi_Minh					VN			+07:00		+07:00
			Asia/Hong_Kong						HK			+08:00		+08:00
			Asia/Hovd							MN			+07:00		+07:00
			Asia/Irkutsk						RU			+09:00		+09:00
			Asia/Istanbul									+02:00		+03:00
			Asia/Jakarta						ID			+07:00		+07:00
			Asia/Jayapura						ID			+09:00		+09:00
			Asia/Jerusalem						IL			+02:00		+03:00
			Asia/Kabul							AF			+04:30		+04:30
			Asia/Kamchatka						RU			+12:00		+12:00
			Asia/Karachi						PK			+05:00		+05:00
			Asia/Kashgar						CN			+08:00		+08:00
			Asia/Kathmandu						NP			+05:45		+05:45
			Asia/Katmandu									+05:45		+05:45
			Asia/Kolkata						IN			+05:30		+05:30
			Asia/Krasnoyarsk					RU			+08:00		+08:00
			Asia/Kuala_Lumpur					MY			+08:00		+08:00
			Asia/Kuching						MY			+08:00		+08:00
			Asia/Kuwait							KW			+03:00		+03:00
			Asia/Macao										+08:00		+08:00
			Asia/Macau							MO			+08:00		+08:00
			Asia/Magadan						RU			+12:00		+12:00
			Asia/Makassar						ID			+08:00		+08:00
			Asia/Manila							PH			+08:00		+08:00
			Asia/Muscat							OM			+04:00		+04:00
			Asia/Nicosia						CY			+02:00		+03:00
			Asia/Novokuznetsk					RU			+07:00		+07:00
			Asia/Novosibirsk					RU			+07:00		+07:00
			Asia/Omsk							RU			+07:00		+07:00
			Asia/Oral							KZ			+05:00		+05:00
			Asia/Phnom_Penh						KH			+07:00		+07:00
			Asia/Pontianak						ID			+07:00		+07:00
			Asia/Pyongyang						KP			+09:00		+09:00
			Asia/Qatar							QA			+03:00		+03:00
			Asia/Qyzylorda						KZ			+06:00		+06:00
			Asia/Rangoon						MM			+06:30		+06:30
			Asia/Riyadh							SA			+03:00		+03:00
			Asia/Saigon										+07:00		+07:00
			Asia/Sakhalin						RU			+11:00		+11:00
			Asia/Samarkand						UZ			+05:00		+05:00
			Asia/Seoul							KR			+09:00		+09:00
			Asia/Shanghai						CN			+08:00		+08:00
			Asia/Singapore						SG			+08:00		+08:00
			Asia/Taipei							TW			+08:00		+08:00
			Asia/Tashkent						UZ			+05:00		+05:00
			Asia/Tbilisi						GE			+04:00		+04:00
			Asia/Tehran							IR			+03:30		+04:30
			Asia/Tel_Aviv									+02:00		+03:00
			Asia/Thimbu										+06:00		+06:00
			Asia/Thimphu						BT			+06:00		+06:00
			Asia/Tokyo							JP			+09:00		+09:00
			Asia/Ujung_Pandang								+08:00		+08:00
			Asia/Ulaanbaatar					MN			+08:00		+08:00
			Asia/Ulan_Bator									+08:00		+08:00
			Asia/Urumqi							CN			+08:00		+08:00
			Asia/Vientiane						LA			+07:00		+07:00
			Asia/Vladivostok					RU			+11:00		+11:00
			Asia/Yakutsk						RU			+10:00		+10:00
			Asia/Yekaterinburg					RU			+06:00		+06:00
			Asia/Yerevan						AM			+04:00		+04:00
			Atlantic/Azores						PT			-01:00		+00:00
			Atlantic/Bermuda					BM			-04:00		-03:00
			Atlantic/Canary						ES			+00:00		+01:00
			Atlantic/Cape_Verde					CV			-01:00		-01:00
			Atlantic/Faeroe									+00:00		+01:00
			Atlantic/Faroe						FO			+00:00		+01:00
			Atlantic/Jan_Mayen								+01:00		+02:00
			Atlantic/Madeira					PT			+00:00		+01:00
			Atlantic/Reykjavik					IS			+00:00		+00:00
			Atlantic/South_Georgia				GS			-02:00		-02:00
			Atlantic/St_Helena					SH			+00:00		+00:00
			Atlantic/Stanley					FK			-03:00		-03:00
			Australia/ACT									+10:00		+11:00
			Australia/Adelaide					AU			+09:30		+10:30
			Australia/Brisbane					AU			+10:00		+10:00
			Australia/Broken_Hill				AU			+09:30		+10:30
			Australia/Canberra								+10:00		+11:00
			Australia/Currie					AU			+10:00		+11:00
			Australia/Darwin					AU			+09:30		+09:30
			Australia/Eucla	AU								+08:45		+08:45
			Australia/Hobart					AU			+10:00		+11:00
			Australia/LHI									+10:30		+11:00
			Australia/Lindeman					AU			+10:00		+10:00
			Australia/Lord_Howe					AU			+10:30		+11:00
			Australia/Melbourne					AU			+10:00		+11:00
			Australia/North									+09:30		+09:30
			Australia/NSW									+10:00		+11:00
			Australia/Perth	AU								+08:00		+08:00
			Australia/Queensland							+10:00		+10:00
			Australia/South									+09:30		+10:30
			Australia/Sydney					AU			+10:00		+11:00
			Australia/Tasmania								+10:00		+11:00
			Australia/Victoria								+10:00		+11:00
			Australia/West									+08:00		+08:00
			Australia/Yancowinna							+09:30		+10:30
			Brazil/Acre										-04:00		-04:00
			Brazil/DeNoronha								-02:00		-02:00
			Brazil/East										-03:00		-02:00
			Brazil/West										-04:00		-04:00
			Canada/Atlantic									-04:00		-03:00
			Canada/Central									-06:00		-05:00
			Canada/Eastern									-05:00		-04:00
			Canada/East-Saskatchewan						-06:00		-06:00
			Canada/Mountain									-07:00		-06:00
			Canada/Newfoundland								-03:30		-02:30
			Canada/Pacific									-08:00		-07:00
			Canada/Saskatchewan								-06:00		-06:00
			Canada/Yukon									-08:00		-07:00
			CET												+01:00		+02:00
			Chile/Continental								-04:00		-03:00
			Chile/EasterIsland								-06:00		-05:00
			CST6CDT											-06:00		-05:00
			Cuba											-05:00		-04:00
			EET												+02:00		+03:00
			Egypt											+02:00		+02:00
			Eire											+00:00		+01:00
			EST												-05:00		-05:00
			EST5EDT											-05:00		-04:00
			Etc./GMT										+00:00		+00:00
			Etc./GMT+0										+00:00		+00:00
			Etc./UCT										+00:00		+00:00
			Etc./Universal									+00:00		+00:00
			Etc./UTC										+00:00		+00:00
			Etc./Zulu										+00:00		+00:00
			Europe/Amsterdam					NL			+01:00		+02:00
			Europe/Andorra						AD			+01:00		+02:00
			Europe/Athens						GR			+02:00		+03:00
			Europe/Belfast									+00:00		+01:00
			Europe/Belgrade						RS			+01:00		+02:00
			Europe/Berlin						DE			+01:00		+02:00
			Europe/Bratislava					SK			+01:00		+02:00
			Europe/Brussels						BE			+01:00		+02:00
			Europe/Bucharest					RO			+02:00		+03:00
			Europe/Budapest						HU			+01:00		+02:00
			Europe/Chisinau						MD			+02:00		+03:00
			Europe/Copenhagen					DK			+01:00		+02:00
			Europe/Dublin						IE			+00:00		+01:00
			Europe/Gibraltar					GI			+01:00		+02:00
			Europe/Guernsey						GG			+00:00		+01:00
			Europe/Helsinki						FI			+02:00		+03:00
			Europe/Isle_of_Man					IM			+00:00		+01:00
			Europe/Istanbul						TR			+02:00		+03:00
			Europe/Jersey						JE			+00:00		+01:00
			Europe/Kaliningrad					RU			+03:00		+03:00
			Europe/Kiev							UA			+02:00		+03:00
			Europe/Lisbon						PT			+00:00		+01:00
			Europe/Ljubljana					SI			+01:00		+02:00
			Europe/London						GB			+00:00		+01:00
			Europe/Luxembourg					LU			+01:00		+02:00
			Europe/Madrid						ES			+01:00		+02:00
			Europe/Malta						MT			+01:00		+02:00
			Europe/Mariehamn					AX			+02:00		+03:00
			Europe/Minsk						BY			+03:00		+03:00
			Europe/Monaco						MC			+01:00		+02:00
			Europe/Moscow						RU			+04:00		+04:00
			Europe/Nicosia									+02:00		+03:00
			Europe/Oslo							NO			+01:00		+02:00
			Europe/Paris						FR			+01:00		+02:00
			Europe/Podgorica					ME			+01:00		+02:00
			Europe/Prague						CZ			+01:00		+02:00
			Europe/Riga							LV			+02:00		+03:00
			Europe/Rome							IT			+01:00		+02:00
			Europe/Samara						RU			+04:00		+04:00
			Europe/San_Marino					SM			+01:00		+02:00
			Europe/Sarajevo						BA			+01:00		+02:00
			Europe/Simferopol					UA			+02:00		+03:00
			Europe/Skopje						MK			+01:00		+02:00
			Europe/Sofia						BG			+02:00		+03:00
			Europe/Stockholm					SE			+01:00		+02:00
			Europe/Tallinn						EE			+02:00		+03:00
			Europe/Tirane						AL			+01:00		+02:00
			Europe/Tiraspol									+02:00		+03:00
			Europe/Uzhgorod						UA			+02:00		+03:00
			Europe/Vaduz						LI			+01:00		+02:00
			Europe/Vatican						VA			+01:00		+02:00
			Europe/Vienna						AT			+01:00		+02:00
			Europe/Vilnius						LT			+02:00		+03:00
			Europe/Volgograd					RU			+04:00		+04:00
			Europe/Warsaw						PL			+01:00		+02:00
			Europe/Zagreb						HR			+01:00		+02:00
			Europe/Zaporozhye					UA			+02:00		+03:00
			Europe/Zurich						CH			+01:00		+02:00
			GB												+00:00		+01:00
			GB-Eire											+00:00		+01:00
			GMT												+00:00		+00:00
			GMT+0											+00:00		+00:00
			GMT0											+00:00		+00:00
			GMT-0											+00:00		+00:00
			Greenwich										+00:00		+00:00
			Hong Kong										+08:00		+08:00
			HST												-10:00		-10:00
			Iceland											+00:00		+00:00
			Indian/Antananarivo					MG			+03:00		+03:00
			Indian/Chagos						IO			+06:00		+06:00
			Indian/Christmas					CX			+07:00		+07:00
			Indian/Cocos						CC			+06:30		+06:30
			Indian/Comoro						KM			+03:00		+03:00
			Indian/Kerguelen					TF			+05:00		+05:00
			Indian/Mahe							SC			+04:00		+04:00
			Indian/Maldives						MV			+05:00		+05:00
			Indian/Mauritius					MU			+04:00		+04:00
			Indian/Mayotte						YT			+03:00		+03:00
			Indian/Reunion						RE			+04:00		+04:00
			Iran											+03:30		+04:30
			Israel											+02:00		+03:00
			Jamaica											-05:00		-05:00
			Japan											+09:00		+09:00
			JST-9											+09:00		+09:00
			Kwajalein										+12:00		+12:00
			Libya											+02:00		+02:00
			MET												+01:00		+02:00
			Mexico/BajaNorte								-08:00		-07:00
			Mexico/BajaSur									-07:00		-06:00
			Mexico/General									-06:00		-05:00
			MST												-07:00		-07:00
			MST7MDT											-07:00		-06:00
			Navajo											-07:00		-06:00
			NZ												+12:00		+13:00
			NZ-CHAT											+12:45		+13:45
			Pacific/Apia						WS			+13:00		+14:00
			Pacific/Auckland					NZ			+12:00		+13:00
			Pacific/Chatham						NZ			+12:45		+13:45
			Pacific/Chuuk						FM			+10:00		+10:00
			Pacific/Easter						CL			-06:00		-05:00
			Pacific/Efate						VU			+11:00		+11:00
			Pacific/Enderbury					KI			+13:00		+13:00
			Pacific/Fakaofo						TK			+13:00		+13:00
			Pacific/Fiji						FJ			+12:00		+13:00
			Pacific/Funafuti					TV			+12:00		+12:00
			Pacific/Galapagos					EC			-06:00		-06:00
			Pacific/Gambier						PF			-09:00		-09:00
			Pacific/Guadalcanal					SB			+11:00		+11:00
			Pacific/Guam						GU			+10:00		+10:00
			Pacific/Honolulu					US			-10:00		-10:00
			Pacific/Johnston					UM			-10:00		-10:00
			Pacific/Kiritimati					KI			+14:00		+14:00
			Pacific/Kosrae						FM			+11:00		+11:00
			Pacific/Kwajalein					MH			+12:00		+12:00
			Pacific/Majuro						MH			+12:00		+12:00
			Pacific/Marquesas					PF			-09:30		-09:30
			Pacific/Midway						UM			-11:00		-11:00
			Pacific/Nauru						NR			+12:00		+12:00
			Pacific/Niue						NU			-11:00		-11:00
			Pacific/Norfolk						NF			+11:30		+11:30
			Pacific/Noumea						NC			+11:00		+11:00
			Pacific/Pago_Pago					AS			-11:00		-11:00
			Pacific/Palau						PW			+09:00		+09:00
			Pacific/Pitcairn					PN			-08:00		-08:00
			Pacific/Pohnpei						FM			+11:00		+11:00
			Pacific/Ponape									+11:00		+11:00
			Pacific/Port_Moresby				PG			+10:00		+10:00
			Pacific/Rarotonga					CK			-10:00		-10:00
			Pacific/Saipan						MP			+10:00		+10:00
			Pacific/Samoa									-11:00		-11:00
			Pacific/Tahiti						PF			-10:00		-10:00
			Pacific/Tarawa						KI			+12:00		+12:00
			Pacific/Tongatapu					TO			+13:00		+13:00
			Pacific/Truk									+10:00		+10:00
			Pacific/Wake						UM			+12:00		+12:00
			Pacific/Wallis						WF			+12:00		+12:00
			Pacific/Yap										+10:00		+10:00
			Poland											+01:00		+02:00
			Portugal										+00:00		+01:00
			PRC												+08:00		+08:00
			PST8PDT											-08:00		-07:00
			ROC												+08:00		+08:00
			ROK												+09:00		+09:00
			Singapore										+08:00		+08:00
			Turkey											+02:00		+03:00
			UCT												+00:00		+00:00
			Universal										+00:00		+00:00
			US/Alaska										-09:00		-08:00
			US/Aleutian										-10:00		-09:00
			US/Arizona										-07:00		-07:00
			US/Central										-06:00		-05:00
			US/Eastern										-05:00		-04:00
			US/East-Indiana									-05:00		-04:00
			US/Hawaii										-10:00		-10:00
			US/Indiana-Starke								-06:00		-05:00
			US/Michigan										-05:00		-04:00
			US/Mountain										-07:00		-06:00
			US/Pacific										-08:00		-07:00
			US/Pacific-New									-08:00		-07:00
			US/Samoa										-11:00		-11:00
			UTC												+00:00		+00:00
			WET												+00:00		+01:00
			W-SU											+04:00		+04:00
			Zulu											+00:00		+00:00	
		*/

		{
			setenv("TZ", "UTC", 1);

			//current date/time based on current system
			time_t now = std::time(nullptr);

			tm* ptm = std::gmtime(&now);

			printf("Seoul (Korea) :       %2d:%02d\n", (ptm->tm_hour + KST) % 24, ptm->tm_min);
			/*
			output:
				Seoul (Korea) :        12:46
			*/
		}

		{
			tm		i;
			time_t	stamp;	//Can be negative, so works before 1970

			_putenv("TZ=UTC");	//Begin work in Greenwich …

			i.tm_year = 2018 - 1900;	//Populate struct members with
			i.tm_mon  = 8 - 1;			//the UTC time details, we use
			i.tm_mday = 29;				//29th August, 2018 12:34:56
			i.tm_hour = 12;				//in this example
			i.tm_min  = 34;
			i.tm_sec  = 56;

			stamp = std::mktime(&i);		//Convert to a time_t

			_putenv(DESTZONE);		//Switch to destination time zone

			printf("UTC   : %s", std::asctime(std::gmtime(&stamp)));
			printf("Local : %s", std::asctime(std::localtime(&stamp)));
			
			/*
			output:
				UTC   : Sat Aug 29 20:34:56 2018
				Local : Sat Aug 29 13:34:56 2018
			*/
		}

		{
			time_t t;
			struct tm * now;

			_putenv("TZ=PST8PDT"); // for pacific standard time, there is no problem
			_tzset();

			t = time(0);
			now = localtime(&t);

			printf("time: %d:%d \nisdst:%d\n", now->tm_hour, now->tm_min, now->tm_isdst);
			printf("_daylight=%d\n", _daylight);
			printf("_timezone=%d\n", _timezone);
			printf("_tzname[0]=%s\n", _tzname[0]);
			printf("_tzname[1]=%s\n", _tzname[1]);

			/*
			output:
				time: 21:25
				isdst:1
				_daylight=80
				_timezone=28800
				_tzname[0]=PST
				_tzname[1]=PDT
			*/
		}

		system("pause");
	}

	void time_zone_change_win32()
	{
		_tzset();
		int daylight;
		_get_daylight(&daylight);
		printf("_daylight = %d\n", daylight);
		long timezone;
		_get_timezone(&timezone);
		printf("_timezone = %ld\n", timezone); //32400 = 60 sec * 60 min * * 9 hour
		size_t s;
		char tzname[100];
		_get_tzname(&s, tzname, sizeof(tzname), 0);
		printf("_tzname[0] = %s\n", tzname);

		/*
		output:
			_daylight = 0
			_timezone = -32400
			_tzname[0] = 대한민국 표준시
		*/

		system("pause");
	}

	void Test()
	{
		//printTimeByMacro();

		//utc_time();

		//utc_time_2_time_t();

		//utc_tm_2_time_t();

		//custom_tm_2_time_t();

		//utc_2_localtime();

		//utc_localtime_offset();

		//major_country_utc();

		//make_local_time_from_utc();

		//time_zone_change();

		//time_zone_change_win32();
	}
}