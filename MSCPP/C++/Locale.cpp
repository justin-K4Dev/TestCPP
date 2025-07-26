#include "stdafx.h"

#include <locale>
#include <clocale>
#include <fstream>
#include <iosfwd>
#include <sstream>      // std::istringstream

namespace Locale
{
	void locale_is()
	{
		/*
			Locale

			Computer users all over the world prefer to interact with their systems using their own language and cultural conventions.
			Cultural differences affect for instance the display of monetary values, of date and time.
			Just think of the way numeric values are formatted in different cultures:
				1,000,000.00 in the US is 1.000.000,00 in Germany and 10,00,000.00 in Nepal.

			If you aim for high international acceptance of your products you must build into your software the flexibility to adapt to varying requirements
			that stem from cultural differences.
			Building into software the potential for worldwide use is called internationalization .
			It is one of the challenges of software development in these days.

			Traditionally, internationalization was achieved by means of C.
			Standards like POSIX and X/Open define locales and wide character input and output for standard C.
			Windows 95 and Windows NT have a C interface, too, the Win32 NLSAPI.

			None of the Win32 NLSAPI interfaces matches any of the standard C interfaces though,
			and locales are thread-specific in Windows whereas they are provided per process in Unix.
			
			These are important differences. The concept and level of support, however, is equivalent.
			There is a common notion of locales, and the services provided cover almost the same range of i18n problems.

			Naturally, C++ cannot stand back. The ISO/ANSI C++ standard defines an extensible framework
			that facilitates internationalization of C++ programs in a portable manner.
			Its main elements are locales and facets.
			This article gives an overview of the locale framework and the standard facets defined by ISO/ANSI C++.

				* Facets are divided into six standard categories:

					category	facet			member functions
					collate		collate			compare, hash, transform
					ctype		ctype			is, narrow, scan_is, scan_not, tolower, toupper, widen
								codecvt			always_noconv, encoding, in, length, max_length, out, unshift
					monetary	moneypunct		curr_symbol, decimal_point, frac_digits, grouping, negative_sign, neg_format, positive_sign, pos_format, thousands_sep
								money_get		get
								money_put		put
					numeric		numpunct		decimal_point, falsename, grouping, thousands_sep, truename
								num_get			get
								num_put			put
					time		time_get		date_order, get_date, get_monthname, get_time, get_weekday, get_year (and get, since C++11)
								time_put		put
					messages	messages		close, get, open

				* Categories constant

					Constant		Explanation
					LC_ALL			selects the entire C locale
					LC_COLLATE		selects the collation category of the C locale
					LC_CTYPE		selects the character classification category of the C locale
					LC_MONETARY		selects the monetary formatting category of the C locale
					LC_NUMERIC		selects the numeric formatting category of the C locale
					LC_TIME			selects the time formatting category of the C locale

				* Locale string format

					language[_country[.charset]]

				* Locale string list

					Default

						locale name		description
						C				Default value: ANSI-C convention (English, 7 bit)
						de_DE			German in Germany
						de_DE.88591		German in Germany with ISO Latin-1 encoding
						de_AT			German in Austria
						de_CH			German in Switzerland
						en_US			English in the United States
						en_GB			English in the UK
						en_AU			English in Australia
						en_CA			English in Canada
						fr_FR			French in France
						fr_CH			French in Switzerland
						fr_CA			French in Canada
						ja_JP.jis		Japanese in Japanese with The Japanese Industrial Standard (JIS) encoding
						ja_JP.sjis		Japanese in Japanese with Shift JIS encoding
						ja_JP.ujis		Japanese in Japanese with UNXIZED JIS encoding
						ja_JP.EUC		Japanese in Japanese with Extened UNIX code encoding
						ko_KR			Korean in Korea
						zh_CN			Chinese in China
						zh_TW			Chinese in Taiwan
						lt_LN.bit7		ISO Latin, 7 bit
						lt_LN.bit8		ISO Latin, 8-bit
						POSIX			POSIX convention (English, 7 bit)

					Windows

						package_name	lang_name				locale					localewin								localewincharset
						af_utf8			Afrikaans				af_ZA.UTF-8				Afrikaans_South							Africa.1252	WINDOWS-1252
						sq_utf8			Albanian				sq_AL.UTF-8				Albanian_Albania.1250					WINDOWS-1250
						ar_utf8			Arabic					ar_SA.UTF-8				Arabic_Saudi Arabia.1256				WINDOWS-1256
						eu_utf8			Basque					eu_ES.UTF-8				Basque_Spain.1252						WINDOWS-1252
						be_utf8			Belarusian				be_BY.UTF-8				Belarusian_Belarus.1251					WINDOWS-1251
						bs_utf8			Bosnian					bs_BA.UTF-8				Bosnian (Latin)							WINDOWS-1250
						bg_utf8			Bulgarian				bg_BG.UTF-8				Bulgarian_Bulgaria.1251					WINDOWS-1251
						ca_utf8			Catalan					ca_ES.UTF-8				Catalan_Spain.1252						WINDOWS-1252
						hr_utf8			Croatian				hr_HR.UTF-8				Croatian_Croatia.1250					WINDOWS-1250
						zh_cn_utf8		Chinese (Simplified)	zh_CN.UTF-8				Chinese_China.936						CP936
						zh_tw_utf8		Chinese (Traditional)	zh_TW.UTF-8				Chinese_Taiwan.950						CP950
						cs_utf8			Czech					cs_CZ.UTF-8				Czech_Czech Republic.1250				WINDOWS-1250
						da_utf8			Danish					da_DK.UTF-8				Danish_Denmark.1252						WINDOWS-1252
						nl_utf8			Dutch					nl_NL.UTF-8				Dutch_Netherlands.1252					WINDOWS-1252
						en_utf8			English					en.UTF-8				English_Australia.1252					-empty string-
						en_us_utf8		English (US)			parent en_utf8 used-	-parent en_utf8 used-					-parent en_utf8 used-
						et_utf8			Estonian				et_EE.UTF-8				Estonian_Estonia.1257					WINDOWS-1257
						fa_utf8			Farsi					fa_IR.UTF-8				Farsi_Iran.1256							WINDOWS-1256
						fil_utf8		Filipino				ph_PH.UTF-8				Filipino_Philippines.1252				WINDOWS-1252
						fi_utf8			Finnish					fi_FI.UTF-8				Finnish_Finland.1252					WINDOWS-1252
						fr_utf8			French					fr_FR.UTF-8 or			French_France.1252						WINDOWS-1252
																fr_CH.UTF-8 or
																fr_BE.UTF-8
						fr_ca_utf8		French (Canada)			fr_CA.UTF-8				French_Canada.1252						-parent fr_utf8 used-
						ga_utf8			Gaelic					ga.UTF-8				Gaelic; Scottish Gaelic					WINDOWS-1252
						gl_utf8			Gallego					gl_ES.UTF-8				Galician_Spain.1252						WINDOWS-1252
						ka_utf8			Georgian				ka_GE.UTF-8				Georgian_Georgia.65001					-empty string-
						de_utf8			German					de_DE.UTF-8				German_Germany.1252						WINDOWS-1252
						de_du_utf8		German (Personal)		de_DE.UTF-8				-parent de_utf8 used-					-parent de_utf8 used-
						el_utf8			Greek					el_GR.UTF-8				Greek_Greece.1253						WINDOWS-1253
						gu_utf8			Gujarati				gu.UTF-8				Gujarati_India.0
						he_utf8			Hebrew					he_IL.utf8				Hebrew_Israel.1255						WINDOWS-1255
						hi_utf8			Hindi					hi_IN.UTF-8				Hindi.65001								-empty string-
						hu_utf8			Hungarian				hu.UTF-8				Hungarian_Hungary.1250					WINDOWS-1250
						is_utf8			Icelandic				is_IS.UTF-8				Icelandic_Iceland.1252					WINDOWS-1252
						id_utf8			Indonesian				id_ID.UTF-8				Indonesian_indonesia.1252				WINDOWS-1252
						it_utf8			Italian					it_IT.UTF-8				Italian_Italy.1252						WINDOWS-1252
						ja_utf8			Japanese				ja_JP.UTF-8				Japanese_Japan.932						CP932
						kn_utf8			Kannada					kn_IN.UTF-8				Kannada.65001							-empty string-
						km_utf8			Khmer					km_KH.UTF-8				Khmer.65001								-empty string-
						ko_utf8			Korean					ko_KR.UTF-8				Korean_Korea.949						EUC-KR
						lo_utf8			Lao						lo_LA.UTF-8				Lao_Laos.UTF-8							WINDOWS-1257
						lt_utf8			Lithuanian				lt_LT.UTF-8				Lithuanian_Lithuania.1257				WINDOWS-1257
						lv_utf8			Latvian					lat.UTF-8				Latvian_Latvia.1257						WINDOWS-1257
						ml_utf8			Malayalam				ml_IN.UTF-8				Malayalam_India.x-iscii-ma				x-iscii-ma
						ms_utf8			Malaysian				ms_MY.UTF-8				Malay_malaysia.1252						WINDOWS-1252
						mi_tn_utf8		Maori (Ngai Tahu)		mi_NZ.UTF-8				Maori.1252								WINDOWS-1252
						mi_wwow_utf8	Maori (Waikoto Uni)		mi_NZ.UTF-8				Maori.1252								WINDOWS-1252
						mn_utf8			Mongolian				mn.UTF-8				Cyrillic_Mongolian.1251					
						no_utf8			Norwegian				no_NO.UTF-8				Norwegian_Norway.1252					WINDOWS-1252
						no_gr_utf8		Norwegian (Primary)		no_NO.UTF-8				-parent no_utf8 used-					-parent no_utf8 used-
						nn_utf8			Nynorsk					nn_NO.UTF-8				Norwegian-Nynorsk_Norway.1252			WINDOWS-1252
						pl_utf8			Polish					pl.UTF-8				Polish_Poland.1250						WINDOWS-1250
						pt_utf8			Portuguese				pt_PT.UTF-8				Portuguese_Portugal.1252				WINDOWS-1252
						pt_br_utf8		Portuguese (Brazil)		pt_BR.UTF-8				Portuguese_Brazil.1252					WINDOWS-1252
						ro_utf8			Romanian				ro_RO.UTF-8				Romanian_Romania.1250					WINDOWS-1250
						ru_utf8			Russian					ru_RU.UTF-8				Russian_Russia.1251						WINDOWS-1251
						sm_utf8			Samoan					mi_NZ.UTF-8				Maori.1252								WINDOWS-1252
						sr_utf8			Serbian					sr_CS.UTF-8				Bosnian(Cyrillic), Serbian (Cyrillic)	WINDOWS-1251
						sk_utf8			Slovak					sk_SK.UTF-8				Slovak_Slovakia.1250					WINDOWS-1250
						sl_utf8			Slovenian				sl_SI.UTF-8				Slovenian_Slovenia.1250					WINDOWS-1250
						so_utf8			Somali					so_SO.UTF-8				not found!								not found!
						es_utf8			Spanish (International)	es_ES.UTF-8				Spanish_Spain.1252						WINDOWS-1252
						sv_utf8			Swedish					sv_SE.UTF-8				Swedish_Sweden.1252						WINDOWS-1252
						tl_utf8			Tagalog					tl.UTF-8				not found!								not found!
						ta_utf8			Tamil					ta_IN.UTF-8				English_Australia.1252
						th_utf8			Thai					th_TH.UTF-8				Thai_Thailand.874						WINDOWS-874
						to_utf8			Tongan					mi_NZ.UTF-8				Maori.1252								WINDOWS-1252
						tr_utf8			Turkish					tr_TR.UTF-8				Turkish_Turkey.1254						WINDOWS-1254
						uk_utf8			Ukrainian				uk_UA.UTF-8				Ukrainian_Ukraine.1251					WINDOWS-1251
						vi_utf8			Vietnamese				vi_VN.UTF-8				Vietnamese_Viet Nam.1258				WINDOWS-1258

		*/
	}

	void locale_c()
	{
		/*
			Locale C

			As a software developer and reader of C++ Users Journal, you may already have some background in the C programming language,
			and the internationalization services provided by the ANSI C library.

			For this reason, let us start with a short recap of the internationalization services provided by the C library,
			and then build on existing knowledge to describe the C++ locales in terms of the C locale.
			
			Internationalization requires that developers consciously design and implement their software
			and avoid hard-coding information or rules that can be localized.
			
			For example, careful developers never assume specific conventions for formatting numeric or monetary values,
			or for displaying date and time, not even for comparing or sorting strings.
			
			For internationalization, all culture and language dependencies need to be represented in a kind of language table.
			Such a table is called a locale.
			A locale in the C library contains support for the several problem domains.
			The information in a C locale is composed of categories.
			Each of the categories represents a set of related information:

				Category			Content 
				LC_NUMERIC			Rules and symbols for numbers 
				LC_TIME				Values for date and time information 
				LC_MONETARY			Rules and symbols for monetary information 
				LC_CTYPE			Character classification and case conversion 
				LC_COLLATE			Collation sequence
				LC_MESSAGE			Formats and values of messages

			Inside a program, the C locale is represented by one or more global data structures.
			The C library provides a functions that use information from those global data structures to adapt their behavior to local conventions.
			Examples of these functions and the information they cover are listed below:

				C locale function							Information covered
				setlocale(), ...							Locale initialization and language information
				isalpha() , isupper() , isdigit() , ...		Character classification
				strftime() , ...							Date and time functions
				strfmon()									Monetary functions
				printf() , scanf() , ...					Number parsing and formatting
				strcoll() , wcscoll() , ...					String collation
				mblen() , mbtowc() , wctomb() , ...			Multibyte functions
				cat_open() , catgets() , cat_close()		Message retrieval
		*/

		//use C locale
		{
			try {
				//set "C" locale to STD.input
				std::cin.imbue(std::locale::classic()); //or std::locale("C");

				//set German in Germany locale to STD.output
				std::cout.imbue(std::locale("German_Germany.1252"));	//if the locale setting is not supported,
																		//an exception is thrown
			}
			catch (std::exception &e) {
				std::cout << "Exception: " << e.what() << std::endl;
			}

			//read and print floating-point values
			double value;
			std::cin >> value;
			std::cout << value << std::endl;

			/*
			input:
				10.5

			output:
				10,5

				-if the "German_Germany.1252" locale setting succeeds: 10,5
			*/
		}

		//use C locale categories
		{
			std::setlocale(LC_ALL, "American_America.1252");		//the C locale will be enabled American
			std::setlocale(LC_NUMERIC, "English_Australia.1252");	//decimal dot will be Australia
			std::setlocale(LC_TIME, "German_Germany.1252");			//date/time formatting will be Germany

			wchar_t str[100];
			std::time_t t = std::time(nullptr);
			std::wcsftime(str, 100, L"%A %c", std::localtime(&t));
			std::wprintf(L"Number: %.2f\nDate: %Ls\n", 3.14, str);

			std::cout.imbue(std::locale("English_Australia.1252"));
			std::locale currLocale = std::cout.getloc();

			std::cout << currLocale.name() << std::endl;

			/*
			output:
				Number: 3.14
				Date: Samstag 05.05.2018 01:36:08
				English_Australia.1252
			*/
		}

		//use C locale for change
		{
			if (std::setlocale(LC_ALL, "C") == nullptr) {
				fprintf(stderr, "Unable to establish locale\n");
				return;
			}
			else {
				time_t system_time = time(NULL);
				char time_text[81];
				strftime(time_text, 80, "%x %A %B %d", localtime(&system_time));
				printf("[%s]\n", time_text);
			}

			if (std::setlocale(LC_ALL, "German_Germany.1252") == nullptr) {
				fprintf(stderr, "Unable to establish locale\n");
				return;
			}
			else {
				time_t system_time = time(NULL);
				char time_text[81];
				strftime(time_text, 80, "%x %A %B %d", localtime(&system_time));
				printf("[%s]\n", time_text);
			}

			/*
			output:
				[05/28/18 Monday May 28]
				[28.05.2018 Montag Mai 28]
			*/
		}

		system("pause");
	}

	void locale_cpp()
	{
		/*
			Locale C++

			In C++, internationalization semantics are broken out into separate classes, so-called facets.
			Each facet offers a set of internationalization services.
			For instance, the formatting of monetary values is encapsulated in the money_put<> facet.
			(Don't get distracted by the template parenthesis; they are added because all facets are class templates.)

			Facets may also represent a set of information about certain culture and language dependencies.
			The rules and symbols for monetary information are an example; they are contained in a facet called moneypunct<>.
			In C++, there is also a class called locale.
			
			Different from a C locale, which is a global data structure representing various culture and language dependencies,
			the C++ class locale is an abstraction that manages facets.

			Basically, you can think of a C++ locale as a container of facets.
			This concept is illustrated graphically below:
			

				ref file : locale_cpp_facets.gif


			The Standard Facets

			The C++ standard defines a number of standard facets.
			They provide services and information similar to those contained in the C library.
			As we have seen, the C locale is composed of six categories of information.
			
			Similarly, there are six groups of standard facets.
			Here is a brief overview:
			
				* Numeric.
				  The facets num_get<charT,InputIterator> and num_put<charT, OutputIterator> handle numeric formatting and parsing.
				  The facets provide get() and put() member functions for values of type long , double , etc.

				  The facet numpunct<charT> specifies numeric formats and punctuation.
				  It provides functions like decimal_point(),thousands_sep(), etc.

				* Monetary.
				  The facets money_get<charT,bool,InputIterator> and money_put<charT, bool, OutputIterator> handle formatting
				  and parsing of monetary values.
				  They provide get() and put() member functions that parse or produce a sequence of digits,
				  representing a count of the smallest unit of the currency.
				  For example, the sequence $1,056.23 in a common US locale would yield 105623 units,
				  or the character sequence "105623".

				  The facet moneypunct <charT, bool International> handles monetary formats
				  and punctuation like the facet numpunct<charT> handles numeric formats and punctuation.
				  It comes with functions like curr_symbol(), etc.
				
				* Time.
				  The facets time_get<charT,InputIterator> and time_put<charT, OutputIterator> handle date and time formatting and parsing.
				  They provide functions like put(), get_time(), get_date(), get_weekday(), etc.
				
				* Ctype.
				  The facet ctype<charT> encapsulates the Standard C++ Library ctype features for character classification, like tolower(), toupper(), is(ctype_base::space,...) etc.
				
				* Collate.
				  The facet collate<charT> provides features for string collation, including a compare() function used for string comparison.
				
				* Code Conversion.
				  The facet codecvt<internT,externT,stateT> is used when converting from one encoding scheme to another,
				  such as from the multibyte encoding JIS to the wide-character encoding Unicode.
				  The main member functions are in() and out().
				
				* Messages.
				  The facet messages<charT> implements message retrieval.
				  It provides facilities to access message catalogues via open() and close(catalog),
				  and to retrieve messages via get(..., int msgid,...).
				  
			As you might have noticed, the names of the standard facets obey certain naming rules.
			The get facets, like num_get and time_get, offer services for parsing.
			The put facets provide formatting services The punct facets, like numpunct and moneypunct, represent rules and symbols.
		*/
	}

	void locale_string_check()
	{
		//use Original
		{
			char* localeStringList[] = {
					"C"
				,	"de_DE"
				,	"de_DE.88591"
				,	"de_AT"
				,	"de_CH"
				,	"en_US"
				,	"en_GB"
				,	"en_AU"
				,	"en_CA"
				,	"fr_FR"
				,	"fr_CH"
				,	"fr_CA"
				,	"ja_JP.jis"
				,	"ja_JP.sjis"
				,	"ja_JP.ujis"
				,	"ja_JP.EUC"
				,	"ko_KR"
				,	"zh_CN"
				,	"zh_TW"
				,	"lt_LN.bit7"
				,	"lt_LN.bit8"
				,	"POSIX"
			};

			for (int i = 0; i < (sizeof(localeStringList) / sizeof(char*)); ++i) {

				try {
					std::cout << "loaded Original locale string: " << localeStringList[i] << std::endl;

					std::setlocale(LC_ALL, localeStringList[i]);	//the C locale will be the UTF-8 enabled English

					wchar_t str[100];
					std::time_t t = std::time(nullptr);
					std::wcsftime(str, 100, L"%A %c", std::localtime(&t));
					std::wprintf(L"Number: %.2f\nDate: %Ls\n", 3.14, str);

					std::locale currLocale = std::cout.getloc();
					std::cout << "Success: " << currLocale.name() << std::endl;
				}
				catch (std::exception &e) {
					std::cout << "Exception: " << e.what() << std::endl;
				}
			}
			/*
			output:
				loaded Original locale string: C
				Number: 3.14
				Date: Monday 05/28/18 02:09:38
				Success: C
				loaded Original locale string: de_DE
				Number: 3.14
				Date: Monday 05/28/18 02:09:41
				Success: C
				loaded Original locale string: de_DE.88591
				Number: 3.14
				Date: Monday 05/28/18 02:09:45
				Success: C
				loaded Original locale string: de_AT
				Number: 3.14
				Date: Monday 05/28/18 02:09:45
				Success: C
				loaded Original locale string: de_CH
				Number: 3.14
				Date: Monday 05/28/18 02:09:45
				Success: C
				loaded Original locale string: en_US
				Number: 3.14
				Date: Monday 05/28/18 02:09:45
				Success: C
				loaded Original locale string: en_GB
				Number: 3.14
				Date: Monday 05/28/18 02:09:45
				Success: C
				loaded Original locale string: en_AU
				Number: 3.14
				Date: Monday 05/28/18 02:09:45
				Success: C
				loaded Original locale string: en_CA
				Number: 3.14
				Date: Monday 05/28/18 02:09:45
				Success: C
				loaded Original locale string: fr_FR
				Number: 3.14
				Date: Monday 05/28/18 02:09:45
				Success: C
				loaded Original locale string: fr_CH
				Number: 3.14
				Date: Monday 05/28/18 02:09:45
				Success: C
				loaded Original locale string: fr_CA
				Number: 3.14
				Date: Monday 05/28/18 02:09:45
				Success: C
				loaded Original locale string: ja_JP.jis
				Number: 3.14
				Date: Monday 05/28/18 02:09:45
				Success: C
				loaded Original locale string: ja_JP.sjis
				Number: 3.14
				Date: Monday 05/28/18 02:09:45
				Success: C
				loaded Original locale string: ja_JP.ujis
				Number: 3.14
				Date: Monday 05/28/18 02:09:45
				Success: C
				loaded Original locale string: ja_JP.EUC
				Number: 3.14
				Date: Monday 05/28/18 02:09:45
				Success: C
				loaded Original locale string: ko_KR
				Number: 3.14
				Date: Monday 05/28/18 02:09:45
				Success: C
				loaded Original locale string: zh_CN
				Number: 3.14
				Date: Monday 05/28/18 02:09:45
				Success: C
				loaded Original locale string: zh_TW
				Number: 3.14
				Date: Monday 05/28/18 02:09:45
				Success: C
				loaded Original locale string: lt_LN.bit7
				Number: 3.14
				Date: Monday 05/28/18 02:09:45
				Success: C
				loaded Original locale string: lt_LN.bit8
				Number: 3.14
				Date: Monday 05/28/18 02:09:45
				Success: C
				loaded Original locale string: POSIX
				Number: 3.14
				Date: Monday 05/28/18 02:09:45
				Success: C
			*/

			system("pause");
		}

		//use Microsoft VC
		{
			char* localeStringList[] = {
					"Afrikaans_South"
				,	"Albanian_Albania.1250"
				,	"Arabic_Saudi Arabia.1256"
				,	"Basque_Spain.1252"
				,	"Belarusian_Belarus.1251"
				,	"Bosnian"
				,	"Bulgarian_Bulgaria.1251"
				,	"Catalan_Spain.1252"
				,	"Croatian_Croatia.1250"
				,	"Chinese_China.936"
				,	"Chinese_Taiwan.950"
				,	"Czech_Czech Republic.1250"
				,	"Danish_Denmark.1252"
				,	"Dutch_Netherlands.1252"
				,	"English_Australia.1252"
				,	"- parent en_utf8 used -"
				,	"Estonian_Estonia.1257"
				,	"Farsi_Iran.1256"
				,	"Filipino_Philippines.1252"
				,	"Finnish_Finland.1252"
				,	"French_France.1252"
				,	"French_Canada.1252"
				,	"Gaelic; Scottish Gaelic"
				,	"Galician_Spain.1252"
				,	"Georgian_Georgia.65001"
				,	"German_Germany.1252"
				,	"- parent de_utf8 used -"
				,	"Greek_Greece.1253"
				,	"Gujarati_India.0"
				,	"Hebrew_Israel.1255"
				,	"Hindi.65001"
				,	"Hungarian_Hungary.1250"
				,	"Icelandic_Iceland.1252"
				,	"Indonesian_indonesia.1252"
				,	"Italian_Italy.1252"
				,	"Japanese_Japan.932"
				,	"Kannada.65001"
				,	"Khmer.65001"
				,	"Korean_Korea.949"
				,	"Lao_Laos.UTF - 8"
				,	"Lithuanian_Lithuania.1257"
				,	"Latvian_Latvia.1257"
				,	"Malayalam_India.x - iscii - ma"
				,	"Malay_malaysia.1252"
				,	"Maori.1252"
				,	"Cyrillic_Mongolian.1251"
				,	"Norwegian_Norway.1252"
				,	"- parent no_utf8 used -"
				,	"Norwegian - Nynorsk_Norway.1252"
				,	"Polish_Poland.1250"
				,	"Portuguese_Portugal.1252"
				,	"Portuguese_Brazil.1252"
				,	"Romanian_Romania.1250"
				,	"Russian_Russia.1251"
				,	"Maori.1252"
				,	"Bosnian(Cyrillic), Serbian(Cyrillic)"
				,	"Slovak_Slovakia.1250"
				,	"Slovenian_Slovenia.1250"
				,	"not found!"
				,	"Spanish_Spain.1252"
				,	"Swedish_Sweden.1252"
				,	"not found!"
				,	"English_Australia.1252"
				,	"Thai_Thailand.874"
				,	"Maori.1252"
				,	"Turkish_Turkey.1254"
				,	"Ukrainian_Ukraine.1251"
				,	"Vietnamese_Viet Nam.1258"
			};

			for (int i = 0; i < (sizeof(localeStringList) / sizeof(char*)); ++i) {

				try {
					std::cout << "loaded Microsoft VC locale string: " << localeStringList[i] << std::endl;
					std::locale newLocale(localeStringList[i]);	//if the locale setting is not supported,
																//an exception is thrown

												//set inputed locale to STD.output
					std::cout.imbue(newLocale);	//if the locale setting is not supported,
												//an exception is thrown

					wchar_t str[100];
					std::time_t t = std::time(nullptr);
					std::wcsftime(str, 100, L"%A %c", std::localtime(&t));
					std::wprintf(L"Number: %.2f\nDate: %Ls\n", 3.14, str);

					std::locale currLocale = std::cout.getloc();
					std::cout << "Success: " << currLocale.name() << std::endl;
				}
				catch (std::exception &e) {
					std::cout << "Exception: " << e.what() << std::endl;
				}
			}
			/*
			output:
				loaded Microsoft VC locale string: Afrikaans_South
				Exception: bad locale name
				loaded Microsoft VC locale string: Albanian_Albania.1250
				Number: 3.14
				Date: Monday 05/28/18 02:29:16
				Success: Albanian_Albania.1250
				loaded Microsoft VC locale string: Arabic_Saudi Arabia.1256
				Number: 3.14
				Date: Monday 05/28/18 02:29:16
				Success: Arabic_Saudi Arabia.1256
				loaded Microsoft VC locale string: Basque_Spain.1252
				Number: 3.14
				Date: Monday 05/28/18 02:29:16
				Success: Basque_Spain.1252
				loaded Microsoft VC locale string: Belarusian_Belarus.1251
				Number: 3.14
				Date: Monday 05/28/18 02:29:16
				Success: Belarusian_Belarus.1251
				loaded Microsoft VC locale string: Bosnian
				Exception: bad locale name
				loaded Microsoft VC locale string: Bulgarian_Bulgaria.1251
				Number: 3.14
				Date: Monday 05/28/18 02:29:16
				Success: Bulgarian_Bulgaria.1251
				loaded Microsoft VC locale string: Catalan_Spain.1252
				Number: 3.14
				Date: Monday 05/28/18 02:29:16
				Success: Catalan_Spain.1252
				loaded Microsoft VC locale string: Croatian_Croatia.1250
				Number: 3.14
				Date: Monday 05/28/18 02:29:16
				Success: Croatian_Croatia.1250
				loaded Microsoft VC locale string: Chinese_China.936
				Number: 3.14
				Date: Monday 05/28/18 02:29:16
				Success: Chinese (Simplified)_People's Republic of China.936
				loaded Microsoft VC locale string: Chinese_Taiwan.950
				Number: 3.14
				Date: Monday 05/28/18 02:29:16
				Success: Chinese (Simplified)_Taiwan.950
				loaded Microsoft VC locale string: Czech_Czech Republic.1250
				Number: 3.14
				Date: Monday 05/28/18 02:29:16
				Success: Czech_Czech Republic.1250
				loaded Microsoft VC locale string: Danish_Denmark.1252
				Number: 3.14
				Date: Monday 05/28/18 02:29:16
				Success: Danish_Denmark.1252
				loaded Microsoft VC locale string: Dutch_Netherlands.1252
				Number: 3.14
				Date: Monday 05/28/18 02:29:16
				Success: Dutch_Netherlands.1252
				loaded Microsoft VC locale string: English_Australia.1252
				Number: 3.14
				Date: Monday 05/28/18 02:29:16
				Success: English_Australia.1252
				loaded Microsoft VC locale string: - parent en_utf8 used -
				Exception: bad locale name
				loaded Microsoft VC locale string: Estonian_Estonia.1257
				Number: 3.14
				Date: Monday 05/28/18 02:29:16
				Success: Estonian_Estonia.1257
				loaded Microsoft VC locale string: Farsi_Iran.1256
				Exception: bad locale name
				loaded Microsoft VC locale string: Filipino_Philippines.1252
				Number: 3.14
				Date: Monday 05/28/18 02:29:16
				Success: Filipino_Philippines.1252
				loaded Microsoft VC locale string: Finnish_Finland.1252
				Number: 3.14
				Date: Monday 05/28/18 02:29:16
				Success: Finnish_Finland.1252
				loaded Microsoft VC locale string: French_France.1252
				Number: 3.14
				Date: Monday 05/28/18 02:29:16
				Success: French_France.1252
				loaded Microsoft VC locale string: French_Canada.1252
				Number: 3.14
				Date: Monday 05/28/18 02:29:16
				Success: French_Canada.1252
				loaded Microsoft VC locale string: Gaelic; Scottish Gaelic
				Exception: bad locale name
				loaded Microsoft VC locale string: Galician_Spain.1252
				Number: 3.14
				Date: Monday 05/28/18 02:29:16
				Success: Galician_Spain.1252
				loaded Microsoft VC locale string: Georgian_Georgia.65001
				Exception: bad locale name
				loaded Microsoft VC locale string: German_Germany.1252
				Number: 3.14
				Date: Monday 05/28/18 02:29:16
				Success: German_Germany.1252
				loaded Microsoft VC locale string: - parent de_utf8 used -
				Exception: bad locale name
				loaded Microsoft VC locale string: Greek_Greece.1253
				Number: 3.14
				Date: Monday 05/28/18 02:29:16
				Success: Greek_Greece.1253
				loaded Microsoft VC locale string: Gujarati_India.0
				Exception: bad locale name
				loaded Microsoft VC locale string: Hebrew_Israel.1255
				Number: 3.14
				Date: Monday 05/28/18 02:29:16
				Success: Hebrew_Israel.1255
				loaded Microsoft VC locale string: Hindi.65001
				Exception: bad locale name
				loaded Microsoft VC locale string: Hungarian_Hungary.1250
				Number: 3.14
				Date: Monday 05/28/18 02:29:16
				Success: Hungarian_Hungary.1250
				loaded Microsoft VC locale string: Icelandic_Iceland.1252
				Number: 3.14
				Date: Monday 05/28/18 02:29:17
				Success: Icelandic_Iceland.1252
				loaded Microsoft VC locale string: Indonesian_indonesia.1252
				Number: 3.14
				Date: Monday 05/28/18 02:29:17
				Success: Indonesian_Indonesia.1252
				loaded Microsoft VC locale string: Italian_Italy.1252
				Number: 3.14
				Date: Monday 05/28/18 02:29:17
				Success: Italian_Italy.1252
				loaded Microsoft VC locale string: Japanese_Japan.932
				Number: 3.14
				Date: Monday 05/28/18 02:29:17
				Success: Japanese_Japan.932
				loaded Microsoft VC locale string: Kannada.65001
				Exception: bad locale name
				loaded Microsoft VC locale string: Khmer.65001
				Exception: bad locale name
				loaded Microsoft VC locale string: Korean_Korea.949
				Number: 3.14
				Date: Monday 05/28/18 02:29:17
				Success: Korean_Korea.949
				loaded Microsoft VC locale string: Lao_Laos.UTF - 8
				Exception: bad locale name
				loaded Microsoft VC locale string: Lithuanian_Lithuania.1257
				Number: 3.14
				Date: Monday 05/28/18 02:29:17
				Success: Lithuanian_Lithuania.1257
				loaded Microsoft VC locale string: Latvian_Latvia.1257
				Number: 3.14
				Date: Monday 05/28/18 02:29:17
				Success: Latvian_Latvia.1257
				loaded Microsoft VC locale string: Malayalam_India.x - iscii - ma
				Exception: bad locale name
				loaded Microsoft VC locale string: Malay_malaysia.1252
				Number: 3.14
				Date: Monday 05/28/18 02:29:17
				Success: Malay_Malaysia.1252
				loaded Microsoft VC locale string: Maori.1252
				Exception: bad locale name
				loaded Microsoft VC locale string: Cyrillic_Mongolian.1251
				Exception: bad locale name
				loaded Microsoft VC locale string: Norwegian_Norway.1252
				Number: 3.14
				Date: Monday 05/28/18 02:29:17
				Success: Norwegian (Bokm?)_Norway.1252
				loaded Microsoft VC locale string: - parent no_utf8 used -
				Exception: bad locale name
				loaded Microsoft VC locale string: Norwegian - Nynorsk_Norway.1252
				Exception: bad locale name
				loaded Microsoft VC locale string: Polish_Poland.1250
				Number: 3.14
				Date: Monday 05/28/18 02:29:17
				Success: Polish_Poland.1250
				loaded Microsoft VC locale string: Portuguese_Portugal.1252
				Number: 3.14
				Date: Monday 05/28/18 02:29:17
				Success: Portuguese_Portugal.1252
				loaded Microsoft VC locale string: Portuguese_Brazil.1252
				Number: 3.14
				Date: Monday 05/28/18 02:29:17
				Success: Portuguese_Brazil.1252
				loaded Microsoft VC locale string: Romanian_Romania.1250
				Number: 3.14
				Date: Monday 05/28/18 02:29:17
				Success: Romanian_Romania.1250
				loaded Microsoft VC locale string: Russian_Russia.1251
				Number: 3.14
				Date: Monday 05/28/18 02:29:17
				Success: Russian_Russia.1251
				loaded Microsoft VC locale string: Maori.1252
				Exception: bad locale name
				loaded Microsoft VC locale string: Bosnian(Cyrillic), Serbian(Cyrillic)
				Exception: bad locale name
				loaded Microsoft VC locale string: Slovak_Slovakia.1250
				Number: 3.14
				Date: Monday 05/28/18 02:29:17
				Success: Slovak_Slovakia.1250
				loaded Microsoft VC locale string: Slovenian_Slovenia.1250
				Number: 3.14
				Date: Monday 05/28/18 02:29:17
				Success: Slovenian_Slovenia.1250
				loaded Microsoft VC locale string: not found!
				Exception: bad locale name
				loaded Microsoft VC locale string: Spanish_Spain.1252
				Number: 3.14
				Date: Monday 05/28/18 02:29:17
				Success: Spanish_Spain.1252
				loaded Microsoft VC locale string: Swedish_Sweden.1252
				Number: 3.14
				Date: Monday 05/28/18 02:29:17
				Success: Swedish_Sweden.1252
				loaded Microsoft VC locale string: not found!
				Exception: bad locale name
				loaded Microsoft VC locale string: English_Australia.1252
				Number: 3.14
				Date: Monday 05/28/18 02:29:17
				Success: English_Australia.1252
				loaded Microsoft VC locale string: Thai_Thailand.874
				Number: 3.14
				Date: Monday 05/28/18 02:29:17
				Success: Thai_Thailand.874
				loaded Microsoft VC locale string: Maori.1252
				Exception: bad locale name
				loaded Microsoft VC locale string: Turkish_Turkey.1254
				Number: 3.14
				Date: Monday 05/28/18 02:29:17
				Success: Turkish_Turkey.1254
				loaded Microsoft VC locale string: Ukrainian_Ukraine.1251
				Number: 3.14
				Date: Monday 05/28/18 02:29:17
				Success: Ukrainian_Ukraine.1251
				loaded Microsoft VC locale string: Vietnamese_Viet Nam.1258
				Exception: bad locale name
			
			*/
			
			system("pause");
		}
	}

	void locale_global()
	{
		/*
			Sets the global locale to loc.

			After the call, the construction of locale objects with its default constructor return a copy of loc.

			If loc has a name, the function also sets the C global locale (as if the C library function setlocale was called with LC_ALL),
			affecting all locale-dependent functions of the C library.
			If loc has no name ("*"), the effect on the C global locale depends on the library implementation.

			Note that the C++ global locale is always changed to loc by this function, no matter its name.
			Therefore, if loc has no name, it may cause the C++ global locale (the one used by locale-dependent functions of the C library)
			to be different from the C++ global locale (the one constructed by default).

			Whether a single global locale exists for the entire program, or one for each thread, depends on the library implementation.
		*/
		{
			std::setlocale(LC_ALL, "C");
			std::locale bar;
			std::cout << "bar locale name: " << bar.name() << std::endl;

			std::locale::global(std::locale("")); //change std::locale object "C" -> "" locale !!!
			std::locale foo;
			std::cout << "foo locale name: " << foo.name() << std::endl;

			// set global locale object for stream 
			std::cin.imbue(std::locale());
			std::cout.imbue(std::locale());
			std::cerr.imbue(std::locale());

			/*
			output:
				bar locale name: C
				foo locale name: Korean_Korea.949
			*/
		}

		{
			std::locale foo;     //the "C" locale
			foo.global(std::locale("")); //change std::locale constructor "C" -> "" locale !!!
			std::locale bar;     //the "" locale

			std::cout << "foo and bar are ";
			std::cout << (foo == bar ? "the same" : "different");
			std::cout << ".\n";

			if (foo != bar) {
				std::cout << "foo locale name: " << foo.name() << std::endl;
				std::cout << "bar locale name: " << bar.name() << std::endl;
			}

			/*
			output:
				foo and bar are different.
				foo locale name: C
				bar locale name: Korean_Korea.949
			*/
		}

		system("pause");
	}

	void locale_c_and_cpp_different()
	{
		/*
			C Locale vs C++ Locales
		
			Apparently, the C locale and the C++ locale along with the standard facets offer similar services.
			However, the semantics of the C++ locale are different from the semantics of the C locale:
			
				* The Standard C locale is a global resource: there is only one locale for the entire application.
			      This makes it hard to build an application that has to handle several locales at a time.
				
				* The Standard C++ locale is a class.
			      Numerous instances of class locale can be c reated at will, so you can have as many locale objects as you need.
			
			To explore this difference in further detail, let us see how locales can be used.
			It may well happen that you have to work with multiple locales.
			
			For example, if you have to implement an application for Switzerland,
			you might want to output messages in Italian, French, and German.
			
			As the C locale is a global data structure, you will have to switch locales several times.
			
			Let's discuss an application that works with multiple locales.
			Say, the application runs at a US company that ships products worldwide.
			
			Our application's responsibility is printing of invoices to be sent to customers all over the world.
			Of course, the invoices need to be printed in the customer's native language.
			
			Say, the application reads input (the product price list) in US English,
			and writes output (the invoice) in the customer's native language, say German.
			
			Since there is only one global locale in C that affects both input and output,
			the global locale must change between input and output operations.


				ref file: locale_c_io.gif
		*/

		//Here is the C code that corresponds to the previous example: 
		{			
			float price;
			
			try {
				std::setlocale(LC_ALL, "American_America.1252");

				FILE* inFile = fopen("./locale_in.txt", "r");
				if (nullptr == inFile) {
					return;
				}

				fscanf(inFile, "%f", &price);

				fclose(inFile);
			}
			catch (std::exception &e) {
				std::cout << "Exception: " << e.what() << std::endl;
			}

			try {
				//convert $ to DM accosrding to the current exchange rate
				std::setlocale(LC_ALL, "German_Germany.1252");

				FILE* outFile = fopen("./locale_out.txt", "w");
				if (nullptr == outFile) {
					return;
				}

				fprintf(outFile, "%f", price);

				fclose(outFile);
			}
			catch (std::exception &e) {
				std::cout << "Exception: " << e.what() << std::endl;
			}

			/*
			input:
				49.99

			output:
				49,990002 in file
			*/

			system("pause");
		}

		/*
			Using C++ locale objects dramatically simplifies the task of using multiple locales.
			The iostreams in the Standard C++ Library are internationalized so that streams can be imbued with separate locale objects.
			For example, the input stream can be imbued with an English locale object,
			and the output stream can be imbued with a German locale object.
			In this way, switching locales becomes unnecessary.


				ref file: locale_cpp_io.gif
		*/

		//Here is the C++ code corresponding to the previous example :
		{
			float price;

			try {
				std::ifstream inFile("./locale_in.txt");
				if (!inFile.is_open()) {
					return;
				}

				inFile.imbue(std::locale("American_America.1252"));

				inFile >> price;
			}
			catch (std::exception &e) {
				std::cout << "Exception: " << e.what() << std::endl;
			}

			try {
				std::ofstream outFile("./locale_out.txt");
				if (!outFile.is_open()) {
					return;
				}

				outFile.imbue(std::locale("German_Germany.1252"));

				//convert $ to DM according to the current exchange rate
				outFile << price;
			}
			catch (std::exception &e) {
				std::cout << "Exception: " << e.what() << std::endl;
			}

			/*
			input:
				49.99

			output:
				49,99 in file
			*/

			system("pause");
		}

		/*
			With these toy examples given above switching locales might look like a minor inconvenience.
			However, consider the need for multiple locales in an application with multiple threads of execution.
			
			Because all threads share one global locale in C, access to the global locale must be serialized by means of mutual exclusion.
			A lot of locking would occur and mostly slow down the program.
			Ideally, you would want to have locales be completely independent of each other.
			Each component shall have a locale of its own, that is unrelated to other locales in your program.
			
			This is what you have in C++.
			You can create infinitely many, independent, light-weight locale objects that you can attach to streams,
			and exchange between components, or pass around as function arguments for instance.


			Relationship between the C Locale and the C++ Locale.

			The C locale and the C++ locales are mostly unrelated.
			There is only one occasion when they effect each other: making a C++ locale global.

			The matter is that there is a global locale in C++, as there is in C.
			You can make a given locale object global by calling locale::global().
			The notion of a global C++ locale was added for all those users who do not want to bother with internationalization
			and rely on internationalized components to pick a sensible default locale.
			
			The global C++ locale is often used as the default locale.
			IOStreams, for instance, uses it; if you do not explicitly imbue your streams with any particular locale object,
			a snapshot of the global locale is used.

			Making a C++ locale object global via locale::global() affects the global C locale in that it results in a call to setlocale().
			When this happens, locale-sensitive C functions called from within a C++ program will use the global C++ locale.
			Conversely, there is no way to affect the C++ locale from within a C program though.
		*/
	}

	void try_lower(const std::ctype<wchar_t>& f, wchar_t c)
	{
		wchar_t up = f.tolower(c);
		if (up != c) {
			std::wcout << "Lower case form of \'" << c << "' is " << up << "\n";
		}
		else {
			std::wcout << '\'' << c << "' has no lower case form\n";
		}
	}

	void locale_cpp_facets()
	{
		/*
			Using C++ Locales and Facets

			After this brief overview of C++ locales and facets let us now explore how they are used.
			Remember, a locale in C++ is a container of facets, and a facet is a set of internationalization services and information.
			
			The general pattern of usage is:
				
				* Create a locale. First, you create a locale object and stuff all the facets you need into the locale.

				* Make available a locale. You can pass around such a locale object to those components that might need it;
				  for instance, you can attach it to a stream and the stream's shift operator will use it.

				* Retrieve a facet. When you need a service from the locale, then you ask the locale to give you a handle to the respective facet
				  that contains the service you need.
			
				* Invoke a service. Via this handle you eventually invoke the facet's service.
			
			This sounds more complicated than it actually is, as we will see later.
			However, it points out that the locale does not know anything about the facets' capabilities.
			The locale only maintains the facets.
			It registers them and makes them accessible on demand.
			The locale, however, does not provide you with the internationalization services itself.
			It only gives you access to facets that provide services.
			It is your task to memorize which facets you need for which particular service.
			The advantage of separating maintenance from functionality is that a locale can maintain any kind of facet,
			not only the predefined standard facets from the C++ library,
			but also novel facets that are added to the library for special purposes.
			
			Creating Locales
			
			Class locale has numerous constructors; see Box 2 for a comprehensive list.
			Basically they fall into three categories:
			
				1. By name. You can create a locale object from a C locale's external representation.
				   Class locale has a constructor std::locale::locale(const char* std_name) that takes the name of a C locale.
				   This locale name is like the one you would use for a call to the C library function std::setlocale().
				   We have already used this constructor in the example above when we created a US English locale by invoking std::locale("En_US");.
				   A locale created this way contains all the standard facets and therefore makes available all services and information equivalent to the C locale you specified.
				
				2. The classic locale. The standard C++ library contains a predefined locale object, std::locale::classic(), which represents the US English ASCII environment.
				   The is the counterpart to the locale named "C" in the C library.

				3. By composition. You can construct a new locale object as a copy of an existing locale object, that has one or several fac et objects replaced.
				   Below are a couple of constructors of class locale that allow creation of locales by composition.
			

				class locale
				{
				public:

					locale(const locale& other, const char* std_name, category);

					template <class Facet> locale(const locale& other, Facet* f);

					template <class Facet> locale(const locale& other, const locale& one);

					locale(const locale& other, const locale& one, category);

				};
			

			The following example uses the first constructor
			and shows how you can construct a locale object as a copy of the classic locale object
			with the classic numeric facets replaced by the numeric facet objects taken from a German locale object.
			
				std::locale loc ( std::locale::classic(), locale("De_DE"), LC_NUMERIC );
			
			The classic locale is created via std::locale::classic(), the German locale is crated via std::locale("De_DE").
			LC_NUMERIC is a locale category.

			As mentioned earlier, the facets fall into categories, and the LC_NUMERIC is the category that designates all numeric facets in a locale.
			Note that some of the constructors are member templates, which is a language feature that is relatively new to the language and not supported by all compilers.

			Immutability of Locales.
			It's important to understand that locales are immutable objects:
				once a locale object is created, it cannot be modified, i.e. no facets can be replaced after construction.
				This makes locales reliable and easy to use and you can safely pass them around between components.

			Copying locales. Copying a locale object is a cheap operation.
			You should have no hesitation about passing locale objects around by value.
			You may copy locale objects for composing new locale objects; you may pass copies of locale objects as arguments to functions, etc.

			Locales are implemented using reference counting and the handle-body-idiom:
				When a locale object is copied, only its handle is duplicated, a fast and inexpensive action.

			The following figure gives an overview of the locale architecture.
			A locale is a handle to a body that maintains a sequence of pointers to facets.
			The facets are reference-counted, too.
		
		
				ref file: locale_cpp_architecture.gif


			Accessing a Locale's Facets
			
			A ccess to the facet objects of a locale object is via two template functions, use_facet and has_facet :

				template <class Facet> const Facet&     use_facet(const locale&);

				template <class Facet> bool             has_facet(const locale&);
			
			The function std::use_facet() is the one that gives access to a facet by providing a constant reference to a facet.
			The function std::has_facet() is for checking whether a certain facet is present in a given locale.
			The requested facet is specified via its type.
			Note, that both functions are template functions.
			The template parameter they take is the type of the facet they try to access in a locale.
			In other words, these function are capable of deciding which facet object is meant from just the information about the facet's type.
			It works because a locale contains at most one exemplar of a certain facet type. This kind of compile-time dispatch is a novel technique in C++.
			A discussion of it and the design of the locale framework's architecture is beyond the scope of this article.
			A detailed description can be found in C++ Report, September 1997, "The Locale Framework" by Klaus Kreft & Angelika Langer.
			
			The code below demonstrates how these functions are used to get access to a facet and invoke an internationalization service.
			It is an example of the conversion service tolower() from the ctype facet;
			all upper case letters of a string read from the standard input stream are converted to lower case letters and are written to the standard output stream.


				std::string in;
				std::cin >> in;

				std::use_facet< ctype<char> >( std::locale::locale() ).tolower( in.c_str(), in.c_str() + in.length() );

				std::cout << in;
			
			The function template std::use_facet< ctype<char> >() returns a constant reference to the locale's facet object.
			Then the facet object's member function tolower() is called. It has the functionality of the C function tolower();
			it converts all upper case letters into lower case letters. A couple of further comments on this example:
			
			Explicit Template Argument Specification.
			The syntax of the call std::use_facet < ctype<char> > (std::locale::locale()) might look surprising to you.
			It is an example of explicit template argument specification, a language feature that is relatively new to C++.
			Template arguments of a function instantiated from a function template can either be explicitly specified in a call or be deduced from the function arguments.
			The explicit template argument specification is needed in the call to std::use_facet above,
			because the compiler can only deduce a template argument if it is the type of one of the function arguments.

			Storing references to facets.
			Note, that we do not store the reference to the facet, but just use the temporary reference returned by use_facet for immediately calling the desired member function of that facet.
			This is a safe way of using facets retrieved from a locale. If you kept the reference, you needed to keep track of the object's lifetime and validity.
			The facet reference does stay valid throughout the lifetime of the locale object it was retrieved from.
			Moreover, the facet referred to does not even change in any way; it is immutable.
			However, when the locale goes out of scope, the references obtained from it might become invalid.
			For this reason it is advisable to combine retrieval and invocation as shown in the example above,
			unless you have a need for doing differently.

			Need for has_facet.
			Note also, that we did not call has_facet< ctype<char> >() in order to check whether the locale has a ctype facet.
			In most situations, you do not have to check for the presence of a standard facet object like ctype<char>.
			This is because locale objects are created by composition; you start with the classic locale or a locale object constructed "by name" from a C locale's external representation.
			Because you can only add or replace facet objects in a locale object, you cannot compose a locale that misses one of the standard facets.
			A call to std::has_facet() is useful, however, when you expect that a certain non-standard facet object should be present in a locale object.
		*/

		//use cpp
		{
			std::locale myLocale(std::locale("American_America.1252"));

			if (true == std::has_facet< std::ctype<wchar_t> >(myLocale)) {

				std::wcout.imbue(myLocale);
				std::wcout << "In American_America.1252 locale:\n";

				auto& f = std::use_facet<std::ctype<wchar_t>>(myLocale);
				try_lower(f, L'S');
				try_lower(f, L'B');
				try_lower(f, L'A');

				std::wstring str = L"HELLo, wORLD!";
				std::wcout << "Lowercase form of the string '" << str << "' is ";
				f.tolower(&str[0], &str[0] + str.size());
				std::wcout << "'" << str << "'\n";
			}
			/*
			ouput:
				In American_America.1252 locale:
				Lower case form of 'S' is s
				Lower case form of 'B' is b
				Lower case form of 'A' is a
				Lowercase form of the string 'HELLo, wORLD!' is 'hello, world!'
			*/
		}

		//use Microsoft VC++
		{
			using namespace std;

			std::locale myLocale("American_America.1252");

			// 표준 방식: facet(ctype<wchar_t>)이 locale에 존재하는지 체크
			if (std::has_facet<std::ctype<wchar_t>>(myLocale)) {
				std::wcout.imbue(myLocale);
				std::wcout << L"In American_America.1252 locale:\n";

				// facet을 표준 방식으로 꺼냄
				const std::ctype<wchar_t>& f = std::use_facet<std::ctype<wchar_t>>(myLocale);

				try_lower(f, L'S');
				try_lower(f, L'B');
				try_lower(f, L'A');

				std::wstring str = L"HELLo, wORLD!";
				std::wcout << L"Lowercase form of the string '" << str << L"' is ";
				f.tolower(&str[0], &str[0] + str.size());
				std::wcout << L"'" << str << L"'\n";
			}
		}

		system("pause");
	}


	template< class tyOStream >
	tyOStream& operator<< (tyOStream& os, const struct tm& date)
	{
		typedef typename tyOStream::char_type				char_t;
		typedef typename tyOStream::traits_type				traits_t;
		typedef std::ostreambuf_iterator<char_t, traits_t>	outIter_t;

		std::locale loc = os.getloc();

		const std::time_put< char_t, outIter_t >& fac = std::use_facet< std::time_put<char_t, outIter_t> >(loc);

		outIter_t nextpos = fac.put(os, os, os.fill(), &date, 'x');
		if (nextpos.failed())
			os.setstate(std::ios_base::badbit);

		return os;
	}

	void locale_cpp_iostream()
	{
		/*
			Locales and IOStreams

			The standard iostreams are an example of an internationalized component that uses locales and facets.
			This feature of iostreams enables you to implement locale-sensitive standard i/o operations for your user-defined types.
			Each stream has a locale object attached.

			Attaching a locale to a stream is done via the stream's imbue() operation.
			If you do not explicitly imbue a locale the stream uses a snapshot of the current global locale as a default.
			
			Here is an example that demonstrates how one can use a stream's locale for printing a date.
			Let us assume we have a date object of type tm, which is the time structure defined in the standard C library, and we want to print it.
			Let's assume our program is supposed to run in a German-speaking canton of Switzerland.
			Hence, we attach a Swiss locale to the standard output stream. When we print the date we expect an output like:		
				1. September 1989 or 01.09.89
		
				tm date; //struct tm

				date.tm_year = 1989;
				date.tm_mon = 9;
				date.tm_mday = 1;

				std::cout.imbue( std::locale::locale("De_CH") );

				std::cout << date;
			
			As there is no operator<<() defined in the Standard C++ Library for the time structure tm from the C library, we have to provide this inserter ourselves.
			The following code suggests a way this can be done.
			To keep it simple, the handling of exceptions thrown during the formatting is omitted.

				template< class tyOStream >
				tyOStream& operator<< (tyOStream& os, const struct tm& date)
				{
					typedef typename tyOStream::char_type			char_t;
					typedef typename tyOStream::traits_type			traits_t;
					typedef ostreambuf_iterator<char_t,traits_t>	outIter_t;

					std::locale loc = os.getloc();

					const std::time_put< char_t,outIter_t >& fac = std::use_facet< std::time_put<char_t, outIter_t> >(loc);

					outIter_t nextpos = fac.put( os, os, os.fill(), &date, 'x' );
					if ( nextpos.failed() )
						os.setstate( std::ios_base::badbit );

					return os;
				}

			There's a lot going on here.
			Let's discuss the interface of the shift operator first.
			The code above shows a typical stream inserter.
			As function arguments it takes a reference to an output stream and a constant reference to the object to be printed.
			It returns a reference to the same stream.
			The inserter is a template function because the standard iostreams are templates; they take a character type
			and an associated traits type describing the character type as template arguments.
			Naturally, we have the same template parameters for our date inserter.

			Now, we need to get hold of the stream's locale object,
			because we want to use its time formatting facet for output of our date object.

			As you can see in the code above, the stream's locale object is obtained via the stream's member function getloc().
			We retrieve the time formatting facet from the locale via std::use_facet; that's an old hat meanwhile.
			We then call the facet's member function put().

			The put() function does all the magic, i.e. it produces a character sequence that represents the equivalent of the date object,
			formatted according to culture-dependent rules and information.
			It then inserts the formatted output into the stream via an output iterator.
			Before we delve into the details of the put() function let us take a look at its return value.

			The put() function returns an output iterator that points to the position immediately after the last inserted character.
			The output iterator used here is an output stream buffer iterator.
			These are special purpose iterators contained in the standard C++ library that bypass the stream's formatting layer
			and write directly to the output stream's underlying stream buffer.

			Output stream buffer iterators have a member function failed() for error indication.
			So we can check for errors happening during the time formatting.
			If there was an error, we set the stream's state accordingly which is done via the stream's setstate() function.

			Let's return to the facet's formatting service put() and see what arguments it takes. Here is the function's interface:

				iter_type put(	iter_type	(a)
							 ,	ios_base&	(b)
							 ,	char_type	(c)
							 ,	const tm*	(d)
							 ,	char	)	(e)

			The types iter_type and char_type stand for the types that were provided as template arguments when the facet class was instantiated.
			In this case, they are ostreambuf_iterator<charT, traits> and charT, where charT and traits are the respective streams template arguments.

				Here is the actual call:
					nextpos = fac.put(os, os, os.fill(), &date, 'x');

			Now let's see what the arguments mean:

				1. The first parameter is supposed to be an output iterator.
				   We provide an iterator to the stream's underlying stream buffer.
				   The reference os to the output stream is converted to an output iterator,
				   because output stream buffer iterators have a constructor taking an output stream, that is, basic_ostream<charT, traits>&.

				2. The second parameter is of type ios_base&, which is one of the stream base classes.
				   The class ios_base contains data for format control (see the section on iostreams for details).
				   The facet object uses this formatting information. We provide the output stream's ios_base part here,
				   using the automatic cast from a reference to an output stream, to a reference to its base class.

				3. The third parameter is the fill character.
				   It is used when the output has to be adjusted and blank characters have to be filled in.
				   We provide the stream's fill character, which one can get by calling the stream's fill() function.

				4. The fourth parameter is a pointer to a time structure tm from the C library.

				5. The fifth parameter is a format character as in the C function strftime();
				   the x stands for the locale's appropriate date representation.

			As you can see from the example of a date inserter function, it is relatively easy to implement powerful,
			locale-sensitive i/o operations using standard iostreams and locale.
			It takes just a couple of lines of C++ code.

			Summary
			This article gave a brief overview of locales and facets - the components in the standard C++ library for support of internationalization of C++ programs.
			The functionality of the standard facets contained in the standard C++ library covers traditional C functionality.
			However, C++ allows multiple locales and overcomes the limitation of one, single global locale that was imposed by C.
			Naturally, this brief introduction to internationalization support in standard C++ is far from being comprehensive.
			For instance, we concealed that locales and facets are designed as an open and extensible frame.
			A description of the framework's architecture and of techniques for extending the framework would fill another article.

			Acknowledgements
			This article is based on material we put together for a book on "Standard C++ IOStreams and Locales" to be published by Addison-Wesley-Longman in 1998.
			Part of the article was inspired by work Angelika Langer did for Rogue Wave Software, Inc. in 1996.
			We also want to thank Nathan Myers, who initially proposed locales and facets to the standards committee.
			He patiently answered countless questions during the past months.
		*/
		{
			tm tDate;
			::memset(&tDate, 0, sizeof(tDate));

			tDate.tm_year = 1;
			tDate.tm_mon = 1;
			tDate.tm_mday = 1;

			std::locale myLocale("German_Germany.1252");

			std::cout.imbue(myLocale);

			std::cout << "Das Datum: " << tDate << std::endl;

			/*
			output:
				Das Datum: 01.02.1901
			*/
		}

		system("pause");
	}

	void multiple_locales_c()
	{
		float price;

		std::setlocale(LC_ALL, "American_America.1252");

		printf("Type in the amount(as float): ");

		scanf("%f", &price);

		//convert $ to DM according to the current exchange rate
		std::setlocale(LC_ALL, "German_Germany.1252");

		printf("\nDer Betrag ist: %f \n", price);

		std::setlocale(LC_ALL, "American_America.1252");

		/*
		input:
			49.99

		output:
			49,99
		*/

		system("pause");
	}

	void multiple_locales_cpp()
	{
		std::cin.imbue(std::locale("American_America.1252"));
		std::cout.imbue(std::locale("German_Germany.1252"));

		float price;

		std::cout << "Type in the amount(as float): ";
		std::cin >> price;

		//convert $ to DM according to the current exchange rate
		std::cout << "\nDer Betrag ist: " << price << std::endl;

		/*
		input:
			49.99

		output:
			49,99
		*/

		system("pause");
	}


	//A useful class to represent an amount of currency.
	//There is no way to change the amount after construction!
	template<typename T>
	struct Currency
	{
		Currency(T value) : _val(value) {}
		T value() const { return _val; }

	private:
		T const _val;
	};

	//Our custom moneypunct facet to format currency as it's done in Germany
	template<typename charT, bool Intl = false>
	struct GermanPunct : public std::moneypunct<charT, Intl>
	{
		GermanPunct(size_t refs = 0) : std::moneypunct<charT, Intl>(refs) {}
		virtual ~GermanPunct() {}

	protected:
		typedef typename std::moneypunct<charT>::string_type string_type;
		typedef typename std::moneypunct<charT>::char_type char_type;
		typedef typename std::moneypunct<charT>::pattern pattern;

		virtual string_type do_curr_symbol() const { return "€"; }
		virtual char_type do_thousands_sep() const { return '.'; }
		virtual std::string do_grouping() const { return "\003"; }
		virtual string_type do_positive_sign() const { return "+"; }
		virtual string_type do_negative_sign() const { return "-"; }
		virtual char_type do_decimal_point() const { return ','; }
		virtual int do_frac_digits() const { return 2; }

		virtual pattern do_pos_format() const
		{
			pattern const p = {
				{
					std::moneypunct<charT>::sign,
					std::moneypunct<charT>::value,
					std::moneypunct<charT>::symbol,
					std::moneypunct<charT>::none
				}
			};
			return p;
		}
		virtual pattern do_neg_format() const
		{
			return do_pos_format();
		}
	};

	//The ostream output operator for our Currency class
	template<typename charT, typename T>
	std::basic_ostream<charT>& operator<<(std::basic_ostream<charT>& o, Currency<T> const& c)
	{
		typedef typename std::basic_ostream<charT>::char_type char_type;
		typedef std::ostreambuf_iterator<char_type, std::char_traits<char_type>> iterator;
		typedef std::money_put<char_type, iterator> money_put;

		std::ostream::sentry s(o);
		if (!s) return o;

		// Locales are reference counted, so that copying them
		// is generally trivial.
		std::locale loc = o.getloc();
		money_put const& mp = std::use_facet<money_put>(loc);

		iterator begin(o);
		mp.put(begin, false, o, ' ', c.value());

		return o;
	}

	void locale_user_define_facet()
	{
		/*
			Facet
			
			They provide ways for formatting and parsing special types such as numbers and currencies.
			Depending on language, culture, and location,
			the same value can be written in different ways or has different units.

			For example, a US bank account might have the following balance:

				$300,104.56

			In Germany, a bank account with the same number of Euros could be printed like this:

				300.104,56€

			Handling these differences is important not only for output,
			but also for reading values from files or other input streams.
			Other local differences apply to sorting text.
			In many Western countries, text is sorted according to an (approximately) 26-letter alphabet.
			But even in those countries, some languages have extra letters or diacritical marks such as ø, ä, ß, í or ü.
			These also need to be sorted in the correct place!

			In the past, J and I were considered to be the same letter in German and sorted accordingly.
			A category of facets called 'collate' is provided to handle such differences.

			Facets are used together with locales.
			They are grouped into six standard categories, each of which has up to three facets:

				* numeric
				* monetary
				* time
				* collate
				* messages
				* ctype
			
			Some categories, such as ctype and numeric, are used automatically.
			Using a locale with a custom numeric facet can change input and output of all numbers.
			Others, such as the monetary category, are available only for use in your own implementation,
			for instance when writing a class for handling currency.
		*/
		{
			//Create a locale based on the current output locale, but with
			//the monetary facet replaced by our custom one.
			//Note that facets are reference-counted by the standard library. 
			//By default they will be destroyed when no references remain.

			std::locale myLoc;
			std::cout << "current locale: " << myLoc.name() << std::endl;

			std::locale loc(std::cout.getloc(), new GermanPunct<char>());
			std::cout.imbue(loc);

			Currency<int> const t(13453334);
			Currency<int> const t2(-13453334);

			//The currency symbol is only displayed when showbase
			//is active.
			std::cout << std::showbase << t << "\n";
			std::cout << std::showbase << t2 << "\n";

			/*
			output:
				current locale: C
				+134.533,34€
				-134.533,34€
			*/
		}

		system("pause");
	}


	void Test()
	{		
		//locale_is();

		//locale_c();

		//locale_cpp();

		//locale_string_check();

		//locale_global();

		//locale_c_and_cpp_different();

		//locale_cpp_facets();

		//locale_cpp_iostream();

		//multiple_locales_c();
		
		//multiple_locales_cpp();

		//locale_user_define_facet();
	}
}