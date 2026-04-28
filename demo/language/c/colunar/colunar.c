/* ***********************************************************************
 * Colunar is a very small lunar calendar under command line.            *
 * It is a lite version of  Lunar 2.2.                                   *
 * This program is distributed under either version 2 of GNU GPL License,*
 * or (at your opinion) any later version of GNU GPL License.            *
 ************************************************************************/

char version[] = "CoLunar Version 0.1 (2010-01-30)";

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

typedef struct {
    int year, month, day, hour, weekday;
    int leap;	/* the lunar month is a leap month */
} Date;

typedef char byte;

#include "tables.h"
static int daysInSolarMonth[13] = {
    0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

static int moon[2] = {29,30}; /* a short (long) lunar month has 29 (30) days */

static	char	*Gan[] = {
    "Jia3",	"Yi3",	 "Bing3", "Ding1", "Wu4",
    "Ji3",	"Geng1", "Xin1",  "Ren2",  "Gui3"
};

static	char	*Zhi[] = {
    "Zi3",	"Chou3", "Yin2",  "Mao3",  "Chen2", "Si4",
    "Wu3",	"Wei4",	 "Shen1", "You3",  "Xu1",   "Hai4"
};

static	char   *ShengXiao[] = {
    "Mouse", "Ox", "Tiger", "Rabbit", "Dragon", "Snake",
    "Horse", "Goat", "Monkey", "Rooster", "Dog", "Pig"
};

static char *weekday[] = {
    "Sunday", "Monday", "Tuesday", "Wednesday",
    "Thursday", "Friday", "Saturday"
};

static	char	*GanHanzi[] = {
    "甲", "乙", "丙", "丁", "戊",
    "己", "庚", "辛", "壬", "癸"
};

static	char	*ZhiHanzi[] = {
    "子", "丑", "寅", "卯", "辰", "巳",
    "午", "未", "申", "酉", "戌", "亥"
};

static	char   *ShengXiaoHanzi[] = {
    "鼠", "牛", "虎", "兔", "龙", "蛇",
    "马", "羊", "猴", "鸡", "狗", "猪"
};

static char *weekdayHanzi[] = {
    "日", "一", "二", "三",
    "四", "五", "六"
};

static char *numHanzi[] = {
    "初一", "初二", "初三", "初四","初五", "初六", "初七", "初八", "初九", "初十",
    "十一", "十二", "十三", "十四","十五", "十六", "十七", "十八", "十九", "二十",
    "廿一", "廿二", "廿三", "廿四","廿五", "廿六", "廿七", "廿八", "廿九", "三十"
};

static char *monthHanzi[] = {
    "正", "杏", "蚕", "槐", "蒲", "伏", 
    "巧", "桂", "玄", "良", "冬", "腊" 
};

Date solar, lunar, gan, zhi, gan2, zhi2, lunar2;

int ymonth[Nyear];	/* number of lunar months in the years */
int yday[Nyear];	/* number of lunar days in the years */
int mday[Nmonth+1];	/* number of days in the months of the lunar year */
int jieAlert;		/* if there is uncertainty in JieQi calculation */

int	showHZ = 1;			/* output in Chinese character */
char *progname;

void Solar2Lunar();
long Solar2Day(), Solar2Day1();
void Day2Lunar(), Day2Solar();
int make_yday(), make_mday(), GZcycle();
void CalGZ();
int	JieDate();
void usage(), Error();
void ReportE();
void ReportHanzi();
void Report();
int CmpDate(int month1, int day1, int month2, int day2);

int main(argc, argv)
    int argc;
    char *argv[];
{
    int year, month, day, hour, i, k, option, inverse=0, leap=0;
    int dateInfo[4];
    struct  tm  *tm_area;
    time_t  clock;

    progname = "colunar";

    for (k=1; k<argc && argv[k][0]=='-'; k++) {
        option = argv[k][1];
        switch(option) {
            case 'e': showHZ = 0; break;
            case 'v': printf("%s\n\n", version);exit(0);
            default:  usage(); break;
        }
    }
    if(argc -k == 0){
        time(&clock);
        tm_area = localtime(&clock);
        tm_area->tm_year += (tm_area->tm_year >= 91) ? 1900:2000;
        tm_area->tm_mon  += 1;
        year = tm_area->tm_year;
        month = tm_area->tm_mon;
        day = tm_area->tm_mday;
        hour =tm_area->tm_hour;
    }
    else {
        if (!((argc - k >= 3) && (argc - k <= 4))) usage();
        dateInfo[3] = 0;
        for (i=0; k<argc && i<4; k++, i++){
            if (sscanf(argv[k], "%d", &dateInfo[i]) != 1)
                usage();
        }
        year = dateInfo[0];
        month = dateInfo[1];
        day = dateInfo[2];
        hour = dateInfo[3];
        if (!(year>=Cyear && year<Cyear+Nyear))
            Error("Year out of range.");
        if (!(month>=1 && month<=12))
            Error("Month out of range.");
        if (!(day>=1 && day<=31) || (inverse && day>30))
            Error("Day out of range.");
        if (!(hour>=0 && hour<=23))
            Error("Hour out of range.");

        if (!inverse && year==SolarFirstDate.year &&
                CmpDate(month, day, SolarFirstDate.month, SolarFirstDate.day)<0)
            Error("Date out of range.");
    }  
    solar.year = year;
    solar.month = month;
    solar.day = day;
    solar.hour = hour;
    Solar2Lunar();

    Report();
    exit(0);
}

void usage()
{
    printf("Usage:\n");
    printf("\tJUST \'%s\' for NOW\n\n", progname);
    printf("\tOR \'colunar [-e] year month day [hour]\'\n");
    printf("\t(in Solar Calendar, 24 hour clock)\n\n");
    printf("\t-e means output in English (Pinyin)\n");
    printf("Date range: about %d years from the Solar Date %d.%d.%d\n", Nyear,
            SolarFirstDate.year, SolarFirstDate.month, SolarFirstDate.day);
    exit(1);
}


void Solar2Lunar()
{

    long offset;
    Date *d;

    offset = Solar2Day(&solar);
    solar.weekday = (offset + SolarFirstDate.weekday) % 7;

    /* A lunar day begins at 11 p.m. */
    if (solar.hour == 23)
        offset++;

    Day2Lunar(offset, &lunar);
    lunar.hour = solar.hour;
    CalGZ(offset, &lunar, &gan, &zhi);

    jieAlert = JieDate(&solar, &lunar2);
    lunar2.day = lunar.day;
    lunar2.hour = lunar.hour;
    CalGZ(offset, &lunar2, &gan2, &zhi2);
}


#define	LeapYear(y)	(((y)%4==0) && ((y)%100!=0) || ((y)%400==0))
#define BYEAR		1201
/* BYEAR % 4 == 1  and BYEAR % 400 == 1 for easy calculation of leap years */
/* assert(BYEAR <= SolarFirstDate.year) */

long Solar2Day(d)
    Date *d;
{
    return (Solar2Day1(d) - Solar2Day1(&SolarFirstDate));
}

/* Compute the number of days from the Solar date BYEAR.1.1 */
long Solar2Day1(d)
    Date *d;
{
    long offset, delta;
    int i;

    delta = d->year - BYEAR;
    if (delta<0) Error("Internal error: pick a larger constant for BYEAR.");
    offset = delta * 365 + delta / 4 - delta / 100 + delta / 400;
    for (i=1; i< d->month; i++)
        offset += daysInSolarMonth[i];
    if ((d->month > 2) && LeapYear(d->year))
        offset++;
    offset += d->day - 1;

    if ((d->month == 2) && LeapYear(d->year)) {
        if (d->day > 29) Error("Day out of range.");
    }
    else if (d->day > daysInSolarMonth[d->month]) Error("Day out of range.");
    return offset;
}


void Day2Lunar(offset, d)
    long offset;
    Date *d;
{
    int i, m, nYear, leapMonth;

    nYear = make_yday();
    for (i=0; i<nYear && offset > 0; i++)
        offset -= yday[i];
    if (offset<0)
        offset += yday[--i];
    if (i==Nyear) Error("Year out of range.");
    d->year = i + LunarFirstDate.year;

    leapMonth = make_mday(i);
    for (m=1; m<=Nmonth && offset>0; m++)
        offset -= mday[m];
    if (offset<0)
        offset += mday[--m];

    d->leap = 0;	/* don't know leap or not yet */

    if (leapMonth>0){	/* has leap month */

        /* if preceeding month number is the leap month,
         * this month is the actual extra leap month */
        d->leap = (leapMonth == (m - 1));

        /* month > leapMonth is off by 1, so adjust it */
        if (m > leapMonth) --m;
    }

    d->month = m;
    d->day = offset + 1;
}


void Day2Solar(offset, d)
    long offset;
    Date *d;
{
    int	i, m, days;

    /* offset is the number of days from SolarFirstDate */
    offset -= Solar2Day(&LunarFirstDate);  /* the argument is negative */
    /* offset is now the number of days from SolarFirstDate.year.1.1 */

    for (i=SolarFirstDate.year;
            (i<SolarFirstDate.year+Nyear) && (offset > 0);	 i++) 
        offset -= 365 + LeapYear(i);
    if (offset<0) {
        --i; 	/* LeapYear is a macro */
        offset += 365 + LeapYear(i);
    }
    if (i==(SolarFirstDate.year + Nyear)) Error("Year out of range.");
    d->year = i;

    /* assert(offset<(365+LeapYear(i))); */
    for (m=1; m<=12; m++) {
        days = daysInSolarMonth[m];
        if ((m==2) && LeapYear(i))	/* leap February */
            days++;
        if (offset<days) {
            d->month = m;
            d->day = offset + 1;
            return;
        }
        offset -= days;
    }
}


int GZcycle(g, z)
    int g, z;
{
    int gz;

    for (gz = z; gz % 10 != g && gz < 60; gz += 12) ;
    if (gz >= 60) printf("internal error\n");
    return gz+1;
}


void CalGZ(offset, d, g, z)
    long offset;
    Date *d, *g, *z;
{
    int	year, month;

    year = d->year - LunarFirstDate.year;
    month = year * 12 + d->month - 1;   /* leap months do not count */

    g->year = (GanFirstDate.year + year) % 10;
    z->year = (ZhiFirstDate.year + year) % 12;
    g->month = (GanFirstDate.month + month) % 10;
    z->month = (ZhiFirstDate.month + month) % 12;
    g->day = (GanFirstDate.day + offset) % 10;
    z->day = (ZhiFirstDate.day + offset) % 12;
    z->hour = ((d->hour + 1) / 2) % 12;
    g->hour = (g->day * 12 + z->hour) % 10;
}


void Error(s)
    char	*s;
{
    printf("%s\n",s);
    exit(1);
}

/* Compare two dates and return <,=,> 0 if the 1st is <,=,> the 2nd */
int CmpDate(month1, day1, month2, day2)
    int month1, day1, month2, day2;
{
    if (month1!=month2) return(month1-month2);
    if (day1!=day2) return(day1-day2);
    return(0);
}

/*
   Given a solar date, find the "lunar" date for the purpose of
   calculating the "4-columns" by taking jie into consideration.
   */
int JieDate(ds, dl)
    Date *ds, *dl;
{
    int m, flag;

    if (ds->month==1) {
        flag = CmpDate(ds->month, ds->day,
                1, fest[ds->year - SolarFirstDate.year - 1][11]);
        if (flag<0) dl->month = 11;
        else if (flag>0) dl->month = 12;
        dl->year = ds->year - 1;
        return(flag==0);
    }
    for (m=2; m<=12; m++) {
        flag = CmpDate(ds->month, ds->day,
                m, fest[ds->year - SolarFirstDate.year][m-2]);
        if (flag==0) m++;
        if (flag<=0) break;
    }
    dl->month = (m-2) % 12;
    dl->year = ds->year;
    if ((dl->month)==0)
    {
        dl->year = ds->year - 1;
        dl->month = 12;
    }
    return(flag==0);
}


/* Compute the number of days in each lunar year in the table */
int make_yday()
{
    int year, i, leap;
    long code;

    for (year = 0; year < Nyear; year++) {
        code = yearInfo[year];
        leap = code & 0xf;
        yday[year] = 0;
        if (leap != 0)
        {
            i = (code >> 16) & 0x1;
            yday[year] += moon[i];
        }
        code >>= 4;
        for (i = 0; i < Nmonth-1; i++) {
            yday[year] += moon[code&0x1];
            code >>= 1;
        }
        ymonth[year] = 12;
        if (leap != 0) ymonth[year]++;
    }
    return Nyear;
}


/* Compute the days of each month in the given lunar year */
int make_mday(year)
    int year;
{
    int i, leapMonth;
    long code;

    code = yearInfo[year];
    leapMonth = code & 0xf;
    /* leapMonth == 0 means no leap month */
    code >>= 4;
    if (leapMonth == 0) {
        mday[Nmonth] = 0;
        for (i = Nmonth-1; i >= 1; i--) {
            mday[i] = moon[code&0x1];
            code >>= 1;
        }
    }
    else {
        /* 
         * There is a leap month (run4 yue4) L in this year.
         * mday[1] contains the number of days in the 1-st month;
         * mday[L] contains the number of days in the L-th month;
         * mday[L+1] contains the number of days in the L-th leap month;
         * mday[L+2] contains the number of days in the L+1 month, etc.	
         * cf. yearInfo[]: info about the leap month is encoded differently.
         * */
        i = (yearInfo[year] >> 16) & 0x1;
        mday[leapMonth+1] = moon[i];
        for (i = Nmonth; i >= 1; i--) {
            if (i == leapMonth+1) i--;
            mday[i] = moon[code&0x1];
            code >>= 1;
        }
    }
    return leapMonth;
}


void Report()
{
    if (showHZ)
        ReportHanzi();
    else
        ReportE();
}


void ReportHanzi()
{
    printf("%s%d%s%2d%s%2d%s%2d%s%s%s\n", "阳历: ",
            solar.year, "年", solar.month, "月", solar.day,
            "日", solar.hour, "时　",
            "星期", weekdayHanzi[solar.weekday]); 
    printf("%s%d%s%s%s%s%s%s%s%s%s%s\n", "阴历: ",
            lunar.year, "年 ", (lunar.leap? " 闰":""),
            monthHanzi[lunar.month-1], "月 ",
            numHanzi[lunar.day-1], " ",
            ZhiHanzi[zhi.hour], "时 ",
            "生肖属", ShengXiaoHanzi[zhi.year]);
    printf("%s%s%s%s%s%s%s%s%s%s%s%s%s\n", "干支: ",
            GanHanzi[gan2.year], ZhiHanzi[zhi2.year], "年 ",
            GanHanzi[gan2.month], ZhiHanzi[zhi2.month], "月 ",
            GanHanzi[gan2.day], ZhiHanzi[zhi2.day], "日 ",
            GanHanzi[gan2.hour], ZhiHanzi[zhi2.hour], "时 ");
    printf("%s%s%s%s%s%s%s%s%s%s%s%s%s\n",
            "用四柱神算推算之时辰八字: \n",
            GanHanzi[gan2.year], ZhiHanzi[zhi2.year], "年 ",
            GanHanzi[gan2.month], ZhiHanzi[zhi2.month], "月 ",
            GanHanzi[gan2.day], ZhiHanzi[zhi2.day], "日 ",
            GanHanzi[gan2.hour], ZhiHanzi[zhi2.hour], "时 ");
    if (jieAlert) {
        printf("* %s, %s\n", "是日为节",
                "月柱可能要修改");
        if (lunar2.month==1)
            printf("* %s\n", "年柱亦可能要修改");
        printf("* %s\n", "请查有节气时间之万年历");
    }
}

void ReportE()
{
    printf("Solar : %d.%d.%d.%d\t%s\n", solar.year, solar.month, solar.day,
            solar.hour, weekday[solar.weekday]);
    printf("Lunar : %d.%s%s.%s.%d\tShengXiao: %s\n",
            lunar.year, monthHanzi[lunar.month], (lunar.leap?"Leap":""), numHanzi[lunar.day],
            lunar.hour, ShengXiao[zhi.year] );
    printf("GanZhi: %s-%s.%s-%s.%s-%s.%s-%s\n",
            Gan[gan.year], Zhi[zhi.year], Gan[gan2.month], Zhi[zhi2.month],
            Gan[gan.day], Zhi[zhi.day], Gan[gan.hour], Zhi[zhi.hour]);
    printf("        (GanZhi Order)\t%d-%d.%d-%d.%d-%d.%d-%d\n",
            gan.year+1, zhi.year+1, gan2.month+1, zhi2.month+1,
            gan.day+1, zhi.day+1, gan.hour+1, zhi.hour+1);
    printf("        (JiaZi Cycle)\t%d.%d.%d.%d\n\n",
            GZcycle(gan.year, zhi.year), GZcycle(gan2.month, zhi2.month),
            GZcycle(gan.day, zhi.day), GZcycle(gan.hour, zhi.hour));
    printf("BaZi (8-characters) according to 'Four Column Calculation':\n");
    printf("        %s-%s.%s-%s.%s-%s.%s-%s\n",
            Gan[gan2.year], Zhi[zhi2.year], Gan[gan2.month], Zhi[zhi2.month],
            Gan[gan2.day], Zhi[zhi2.day], Gan[gan2.hour], Zhi[zhi2.hour]);
    printf("        (GanZhi Order)\t%d-%d.%d-%d.%d-%d.%d-%d\n",
            gan2.year+1, zhi2.year+1, gan2.month+1, zhi2.month+1,
            gan2.day+1, zhi2.day+1, gan2.hour+1, zhi2.hour+1);
    printf("        (JiaZi Cycle)\t%d.%d.%d.%d\n\n",
            GZcycle(gan2.year, zhi2.year), GZcycle(gan2.month, zhi2.month),
            GZcycle(gan2.day, zhi2.day), GZcycle(gan2.hour, zhi2.hour));
    if (jieAlert) {
        printf("* The month column may need adjustment because the date falls on a jie.\n");
        if (lunar2.month==1)
            printf("* The day column may need adjustment, too.\n");
        printf("* Please consult a detailed conversion table.\n");
    }
}
