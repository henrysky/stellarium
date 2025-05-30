/*
 * Stellarium
 * Copyright (C) 2019 Alexander Wolf
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Suite 500, Boston, MA  02110-1335, USA.
 */

#include "tests/testComputations.hpp"

#include <QString>
#include <QDebug>
#include <QtGlobal>
#include <QVariantList>
#include <QMap>

#include "StelUtils.hpp"
#include "VecMath.hpp"

#define ERROR_LOW_LIMIT 1e-3
#define ERROR_MID_LIMIT 1e-4
#define ERROR_HIGH_LIMIT 1e-5

QTEST_GUILESS_MAIN(TestComputations)

void TestComputations::testJDFromBesselianEpoch()
{
	QVariantList data;

	// According to Observational Astrophysics by Pierre Lena, Francois Lebrun, Francois Mignard (ISBN 3662036851, 9783662036853)
	data << 1900.0		<< 2415020.3135;
	data << 1950.0		<< 2433282.4235;
	data << 1995.0004862412	<< 2449718.5;
	data << 1995.0		<< 2449718.3224;
	data << 2000.0		<< 2451544.5334;
	data << 1950.000210	<< 2433282.5;
	data << 2000.001278	<< 2451545.0;

	while (data.count() >= 2)
	{
		double besselianEpoch	= data.takeFirst().toDouble();
		double expectedJD	= data.takeFirst().toDouble();
		double JD = StelUtils::getJDFromBesselianEpoch(besselianEpoch);

		QVERIFY2(qAbs(JD-expectedJD)<=ERROR_LOW_LIMIT, qPrintable(QString("Julian date for Besselian epoch %1 is %2 (expected %3)")
			.arg(QString::number(besselianEpoch, 'f', 6), QString::number(JD, 'f', 4), QString::number(expectedJD, 'f', 4))));
	}
}

void TestComputations::testIsPowerOfTwo()
{
	QVariantList data;
	data	<< 0	<< true;
	data	<< 1	<< true;
	data	<< 2	<< true;
	data	<< 3	<< false;
	data	<< 4	<< true;
	data	<< 5	<< false;
	data	<< 6	<< false;
	data	<< 7	<< false;
	data	<< 8	<< true;
	data	<< 9	<< false;

	while (data.count() >= 2)
	{
		int n	= data.takeFirst().toInt();
		bool f	= data.takeFirst().toBool();

		bool r = StelUtils::isPowerOfTwo(n);

		QVERIFY2(r==f, qPrintable(QString("%1 %2 power of two (expected: %3)")
			.arg(n).arg(r ? "is":"is not").arg(f)));
	}
}

void TestComputations::testGetBiggerPowerOfTwo()
{
	QVariantList data;
	data	<< 0	<< 1;
	data	<< 1	<< 1;
	data	<< 2	<< 2;
	data	<< 3	<< 4;
	data	<< 4	<< 4;
	data	<< 5	<< 8;
	data	<< 6	<< 8;
	data	<< 7	<< 8;
	data	<< 8	<< 8;
	data	<< 9	<< 16;
	data	<< 10	<< 16;
	data	<< 11	<< 16;
	data	<< 12	<< 16;
	data	<< 13	<< 16;
	data	<< 14	<< 16;
	data	<< 15	<< 16;
	data	<< 16	<< 16;

	while (data.count() >= 2)
	{
		int n	= data.takeFirst().toInt();
		int e	= data.takeFirst().toInt();
		int r	= StelUtils::getBiggerPowerOfTwo(n);

		QVERIFY2(r==e, qPrintable(QString("Number: %1 getBiggerPowerOfTwo(): %2 (expected: %3)")
			.arg(n).arg(r).arg(e)));
	}
}

void TestComputations::testDayInYear()
{
	QVariantList data;
	data	<< 2000	<< 1	<< 1	<< 1;
	data	<< 2019	<< 1	<< 1	<< 1;
	data	<< 2019	<< 1	<< 31	<< 31;
	data	<< 2019	<< 2	<< 28	<< 59;
	data	<< 2019	<< 3	<< 10	<< 69;
	data	<< 2020	<< 3	<< 10	<< 70;

	while (data.count() >= 4)
	{
		int year	= data.takeFirst().toInt();
		int month	= data.takeFirst().toInt();
		int day	= data.takeFirst().toInt();
		int en	= data.takeFirst().toInt();

		int n = StelUtils::dayInYear(year, month, day);

		QVERIFY2(n==en, qPrintable(QString("%1-%2-%3 is %4 day (expected: %5)")
				.arg(year).arg(month).arg(day).arg(n).arg(en)));
	}
}

void TestComputations::testYearFraction()
{
	QVariantList data;
	data	<< 2000	<< 1	<< 1	<< 2000.00274;
	data	<< 2019	<< 1	<< 1	<< 2019.00274;
	data	<< 2019	<< 1	<< 31	<< 2019.08493;
	data	<< 2019	<< 2	<< 28	<< 2019.16164;
	data	<< 2019	<< 3	<< 10	<< 2019.18904;
	data	<< 2020	<< 3	<< 10	<< 2020.19126;

	while (data.count() >= 4)
	{
		int year	= data.takeFirst().toInt();
		int month	= data.takeFirst().toInt();
		int day	= data.takeFirst().toInt();
		double efrac	= data.takeFirst().toDouble();

		double fraction = StelUtils::yearFraction(year, month, day);

		QVERIFY2(qAbs(fraction-efrac)<=ERROR_HIGH_LIMIT, qPrintable(QString("%1-%2-%3 is %4 (expected: %5)")
			.arg(year).arg(month).arg(day).arg(QString::number(fraction, 'f', 5), QString::number(efrac, 'f', 5))));
	}
}

void TestComputations::testEquToEqlTransformations()
{
	const double eps0 = 23.4392911*M_PI/180.;

	QVariantList data;
	//                  RA                   DE                     Lambda           Beta
	data <<     0.		<<    0.		<<     0.		<<    0.;
	data <<   12.		<<  45.		<<   31.03451	<<  36.17676;
	data <<     2.5		<<  23.5		<<   12.03425	<<  20.48324;
	data <<   25.1225	<< -46.0025	<<    -1.41879	<< -51.01554;
	data <<  -60.		<<  60.		<<  -11.92479	<<  75.19595;

	while (data.count() >= 4)
	{
		double ra	= data.takeFirst().toDouble();
		double dec	= data.takeFirst().toDouble();
		double lambdaE	= data.takeFirst().toDouble();
		double betaE	= data.takeFirst().toDouble();

		double lambda, beta;
		StelUtils::equToEcl(ra*M_PI/180., dec*M_PI/180., eps0, &lambda, &beta);

		lambda *= 180/M_PI;
		beta *= 180/M_PI;

		QVERIFY2(qAbs(lambda-lambdaE)<=ERROR_LOW_LIMIT && qAbs(lambda-lambdaE)<=ERROR_LOW_LIMIT,
			qPrintable(QString("RA/Dec: %1/%2 Lam/Bet: %3/%4 (expected Lam/Bet: %5/%6)")
			.arg(QString::number(ra, 'f', 5), QString::number(dec, 'f', 5), QString::number(lambda, 'f', 5),
			     QString::number(beta, 'f', 5), QString::number(lambdaE, 'f', 5), QString::number(betaE, 'f', 5))));
	}
}

void TestComputations::testEclToEquTransformations()
{
	const double eps0 = 23.4392911*M_PI/180.;

	QVariantList data;
	//                  RA                   DE                     Lambda           Beta
	data <<     0.		<<    0.		<<     0.		<<    0.;
	data <<   12.		<<  45.		<<   31.03451	<<  36.17676;
	data <<     2.5		<<  23.5		<<   12.03425	<<  20.48324;
	data <<   25.1225	<< -46.0025	<<    -1.41879	<< -51.01554;
	data <<  -60.		<<  60.		<<  -11.92479	<<  75.19595;

	while (data.count() >= 4)
	{
		double raE	= data.takeFirst().toDouble();
		double decE	= data.takeFirst().toDouble();
		double lambda	= data.takeFirst().toDouble();
		double beta	= data.takeFirst().toDouble();

		double ra, dec;
		StelUtils::eclToEqu(lambda*M_PI/180., beta*M_PI/180., eps0, &ra, &dec);

		ra *= 180/M_PI;
		dec *= 180/M_PI;

		QVERIFY2(qAbs(ra-raE)<=ERROR_LOW_LIMIT && qAbs(dec-decE)<=ERROR_LOW_LIMIT,
			qPrintable(QString("Lam/Bet: %1/%2 RA/Dec: %3/%4 (expected RA/Dec: %5/%6)")
			.arg(QString::number(lambda, 'f', 5), QString::number(beta, 'f', 5), QString::number(ra, 'f', 5),
			     QString::number(dec, 'f', 5), QString::number(raE, 'f', 5), QString::number(decE, 'f', 5))));
	}
}

void TestComputations::testSpheToRectTransformations()
{
	const double r = 1.;

	QVariantList data;
	//     Longitude          Latitude         Expected
	data <<     0.		<<   0.		<< "1,0,0";
	data <<    90.		<<   0.		<< "0,1,0";
	data <<     0.		<<  90.		<< "0,0,1";
	data <<   -90.		<<  90.		<< "0,0,1";
	data <<   180.		<< -90.		<< "0,0,-1";
	data <<   270.		<< -90.		<< "0,0,-1";
	data <<   -90.		<<  45.		<< "0,-0.7071,0.7071";

	while (data.count() >= 3)
	{
		double longitude	= data.takeFirst().toDouble();
		double latitude	= data.takeFirst().toDouble();
		float longitudeF1	= float(longitude);
		float latitudeF1	= float(latitude);
		float longitudeF2	= float(longitude);
		float latitudeF2	= float(latitude);
		Vec3f eVec3f		= Vec3f(data.takeFirst().toString());
		Vec3d eVec3d		= eVec3f.toVec3d();

		Vec3f rVec3f;
		Vec3d rVec3d1, rVec3d2, rVec3d1r;
		StelUtils::spheToRect(longitudeF1*M_PI_180f, latitudeF1*M_PI_180f, rVec3f);
		StelUtils::spheToRect(longitudeF2*M_PI_180f, latitudeF2*M_PI_180f, rVec3d2);
		StelUtils::spheToRect(longitude*M_PI/180.,   latitude*M_PI/180.,   rVec3d1);

		StelUtils::spheToRect(longitude*M_PI/180., latitude*M_PI/180., r, rVec3d1r);

		QVERIFY2(qAbs(rVec3f[0]-eVec3f[0])<=ERROR_HIGH_LIMIT && qAbs(rVec3f[1]-eVec3f[1])<=ERROR_HIGH_LIMIT && qAbs(rVec3f[2]-eVec3f[2])<=ERROR_HIGH_LIMIT,
				qPrintable(QString("Long/Lat: %1/%2 = %3 (expected %4)")
				.arg(QString::number(longitude, 'f', 4), QString::number(latitude, 'f', 4),
				     rVec3f.toString(), eVec3f.toString())));

		QVERIFY2(qAbs(rVec3d2[0]-eVec3d[0])<=ERROR_HIGH_LIMIT && qAbs(rVec3d2[1]-eVec3d[1])<=ERROR_HIGH_LIMIT && qAbs(rVec3d2[2]-eVec3d[2])<=ERROR_HIGH_LIMIT,
				qPrintable(QString("Long/Lat: %1/%2 = %3 (expected %4)")
				.arg(QString::number(longitude, 'f', 4), QString::number(latitude, 'f', 4),
				     rVec3d2.toString(), eVec3d.toString())));

		QVERIFY2(qAbs(rVec3d1[0]-eVec3d[0])<=ERROR_HIGH_LIMIT && qAbs(rVec3d1[1]-eVec3d[1])<=ERROR_HIGH_LIMIT && qAbs(rVec3d1[2]-eVec3d[2])<=ERROR_HIGH_LIMIT,
				qPrintable(QString("Long/Lat: %1/%2 = %3 (expected %4)")
				.arg(QString::number(longitude, 'f', 4), QString::number(latitude, 'f', 4),
				     rVec3d1.toString(), eVec3d.toString())));

		QVERIFY2(qAbs(rVec3d1r[0]-eVec3d[0])<=ERROR_HIGH_LIMIT && qAbs(rVec3d1r[1]-eVec3d[1])<=ERROR_HIGH_LIMIT && qAbs(rVec3d1r[2]-eVec3d[2])<=ERROR_HIGH_LIMIT,
				qPrintable(QString("Long/Lat: %1/%2 = %3 (expected %4)")
				.arg(QString::number(longitude, 'f', 4), QString::number(latitude, 'f', 4),
				     rVec3d1r.toString(), eVec3d.toString())));
	}
}

void TestComputations::testRectToSpheTransformations()
{
	QVariantList data;
	//     Longitude          Latitude         Source
	data <<     0.		<<   0.		<< "1,0,0";
	data <<    90.		<<   0.		<< "0,1,0";
	data <<     0.		<<  90.		<< "0,0,1";
	data <<     0.		<< -90.		<< "0,0,-1";
	data <<   -90.		<<  45.		<< "0,-0.7071,0.7071";

	while (data.count() >= 3)
	{
		float longitudeE	= data.takeFirst().toFloat();
		float latitudeE	= data.takeFirst().toFloat();
		Vec3f rVec3f		= Vec3f(data.takeFirst().toString());
		Vec3d rVec3d		= rVec3f.toVec3d();
		Vec3d rVec3d1r	= rVec3d;

		float longitude, latitude, longitudef, latitudef;
		double longituded, latituded, longitudes, latitudes, longitude1r, latitude1r, r = 1.;
		StelUtils::rectToSphe(&longitude,  &latitude,  rVec3f);
		StelUtils::rectToSphe(&longitudef, &latitudef, rVec3d);
		StelUtils::rectToSphe(&longituded, &latituded, rVec3d);
		StelUtils::rectToSphe(&longitudes, &latitudes, rVec3f);

		StelUtils::rectToSphe(&longitude1r, &latitude1r, &r, rVec3d1r);

		longitude	*= M_180_PIf;
		longitudef	*= M_180_PIf;
		longituded	*= M_180_PI;
		longitudes	*= M_180_PI;
		latitude	*= M_180_PIf;
		latitudef	*= M_180_PIf;
		latituded	*= M_180_PI;
		latitudes	*= M_180_PI;

		longitude1r	*= M_180_PI;
		latitude1r	*= M_180_PI;

		QVERIFY2(qAbs(longitude-longitudeE)<=ERROR_HIGH_LIMIT && qAbs(latitude-latitudeE)<=ERROR_HIGH_LIMIT,
				qPrintable(QString("Vec3 %1 = Long/Lat: %2/%3 (expected %4/%5)")
				.arg(rVec3f.toString(), QString::number(longitude, 'f', 4), QString::number(latitude, 'f', 4),
				     QString::number(longitudeE, 'f', 4), QString::number(latitudeE, 'f', 4))));

		QVERIFY2(qAbs(longitudef-longitudeE)<=ERROR_HIGH_LIMIT && qAbs(latitudef-latitudeE)<=ERROR_HIGH_LIMIT,
				qPrintable(QString("Vec3 %1 = Long/Lat: %2/%3 (expected %4/%5)")
				.arg(rVec3d.toString(), QString::number(longitudef, 'f', 4), QString::number(latitudef, 'f', 4),
				     QString::number(longitudeE, 'f', 4), QString::number(latitudeE, 'f', 4))));

		QVERIFY2(qAbs(longituded-(double)longitudeE)<=ERROR_HIGH_LIMIT && qAbs(latituded-(double)latitudeE)<=ERROR_HIGH_LIMIT,
				qPrintable(QString("Vec3 %1 = Long/Lat: %2/%3 (expected %4/%5)")
				.arg(rVec3d.toString(), QString::number(longituded, 'f', 4), QString::number(latituded, 'f', 4),
				     QString::number(longitudeE, 'f', 4), QString::number(latitudeE, 'f', 4))));

		QVERIFY2(qAbs(longitudes-(double)longitudeE)<=ERROR_HIGH_LIMIT && qAbs(latitudes-(double)latitudeE)<=ERROR_HIGH_LIMIT,
				qPrintable(QString("Vec3 %1 = Long/Lat: %2/%3 (expected %4/%5)")
				.arg(rVec3f.toString(), QString::number(longitudes, 'f', 4), QString::number(latitudes, 'f', 4),
				     QString::number(longitudeE, 'f', 4), QString::number(latitudeE, 'f', 4))));

		QVERIFY2(qAbs(longitude1r-(double)longitudeE)<=ERROR_HIGH_LIMIT && qAbs(latitude1r-(double)latitudeE)<=ERROR_HIGH_LIMIT,
				qPrintable(QString("Vec3 %1 = Long/Lat: %2/%3 (expected %4/%5)")
				.arg(rVec3d1r.toString(), QString::number(longitude1r, 'f', 4), QString::number(latitude1r, 'f', 4),
				     QString::number(longitudeE, 'f', 4), QString::number(latitudeE, 'f', 4))));
	}
}

void TestComputations::testVector2Operators()
{
	Vec2f firstF, secondF, vecF, sumF, diffF, mulF, smF, cwmF;
	Vec2d firstD, secondD, vecD, sumD, diffD, mulD, smD, cwmD;
	Vec2i firstI, secondI, vecI, sumI, diffI, mulI, smI, cwmI;
	bool expected, result;
	QVariantList data;
	//           Vec(1)       Vec(2)      Compare   Sum         Diff                   Mul                  Scal. M.           C.W.M.
	data << "0,0"	<< "0,0"	<< true		<<  "0,0"	<<   "0,0"	<<   "0,0"	<<  "0,0"		<< "0,0";
	data << "0,0"	<< "1,0"	<< false	<<  "1,0"	<<  "-1,0"	<<   "0,0"	<<  "0,0"		<< "0,0";
	data << "0,0"	<< "1,1"	<< false	<<  "1,1"	<< "-1,-1"	<<   "0,0"	<<  "0,0"		<< "0,0";
	data << "1,1"	<< "0,0"	<< false	<<  "1,1"	<<   "1,1"	<<   "0,0"	<<  "2,2"		<< "1,1";
	data << "1,1"	<< "1,1"	<< true		<<  "2,2"	<<   "0,0"	<<   "1,1"	<<  "2,2"		<< "1,1";
	data << "1,-1"	<< "1,-1"	<< true		<< "2,-2"	<<   "0,0"	<<   "1,1"	<< "2,-2"		<< "1,1";
	data << "1,-1"	<< "-1,1"	<< false	<<  "0,0"	<<   "2,-2"	<< "-1,-1"	<< "2,-2"		<< "1,1";

	while (data.count() >= 8)
	{
		firstF		= Vec2f(data.takeFirst().toString());
		secondF		= Vec2f(data.takeFirst().toString());
		vecF		= firstF;
		expected	= data.takeFirst().toBool();
		sumF		= Vec2f(data.takeFirst().toString());
		diffF		= Vec2f(data.takeFirst().toString());
		mulF		= Vec2f(data.takeFirst().toString());
		smF		= Vec2f(data.takeFirst().toString());
		cwmF		= Vec2f(data.takeFirst().toString());

		firstD.set((double)firstF[0], (double)firstF[1]);
		vecD		= firstD;
		secondD.set((double)secondF[0], (double)secondF[1]);
		sumD.set((double)sumF[0], (double)sumF[1]);
		diffD.set((double)diffF[0], (double)diffF[1]);
		mulD.set((double)mulF[0], (double)mulF[1]);
		smD.set((double)smF[0], (double)smF[1]);
		cwmD.set((double)cwmF[0], (double)cwmF[1]);

		firstI.set((int)firstF[0], (int)firstF[1]);
		vecI		= firstI;
		secondI.set((int)secondF[0], (int)secondF[1]);
		sumI.set((int)sumF[0], (int)sumF[1]);
		diffI.set((int)diffF[0], (int)diffF[1]);
		mulI.set((int)mulF[0], (int)mulF[1]);
		smI.set((int)smF[0], (int)smF[1]);
		cwmI.set((int)cwmF[0], (int)cwmF[1]);

		result = (firstF==secondF);
		QVERIFY2(result==expected, qPrintable(QString("%1 == %2 (result: %3; expected: %4)")
			.arg(firstF.toString(), secondF.toString()).arg(result).arg(expected)));
		result = (firstD==secondD);
		QVERIFY2(result==expected, qPrintable(QString("%1 == %2 (result: %3; expected: %4)")
			.arg(firstD.toString(), secondD.toString()).arg(result).arg(expected)));
		result = (firstI==secondI);
		QVERIFY2(result==expected, qPrintable(QString("%1 == %2 (result: %3; expected: %4)")
			.arg(firstI.toString(), secondI.toString()).arg(result).arg(expected)));

		result = (firstF!=secondF);
		QVERIFY2(result!=expected, qPrintable(QString("%1 != %2 (result: %3; expected: %4)")
			.arg(firstF.toString(), secondF.toString()).arg(result).arg(expected)));
		result = (firstD!=secondD);
		QVERIFY2(result!=expected, qPrintable(QString("%1 != %2 (result: %3; expected: %4)")
			.arg(firstD.toString(), secondD.toString()).arg(result).arg(expected)));
		result = (firstI!=secondI);
		QVERIFY2(result!=expected, qPrintable(QString("%1 != %2 (result: %3; expected: %4)")
			.arg(firstI.toString(), secondI.toString()).arg(result).arg(expected)));

		Vec2f sumFR = firstF+secondF;
		QVERIFY2(sumFR==sumF, qPrintable(QString("%1 + %2 = %3 (expected: %4)")
			.arg(firstF.toString(), secondF.toString(), sumFR.toString(), sumF.toString())));
		Vec2d sumDR = firstD+secondD;
		QVERIFY2(sumDR==sumD, qPrintable(QString("%1 + %2 = %3 (expected: %4)")
			.arg(firstD.toString(), secondD.toString(), sumDR.toString(), sumD.toString())));
		Vec2i sumIR = firstI+secondI;
		QVERIFY2(sumIR==sumI, qPrintable(QString("%1 + %2 = %3 (expected: %4)")
			.arg(firstI.toString(), secondI.toString(), sumIR.toString(), sumI.toString())));

		Vec2f diffFR = firstF-secondF;
		QVERIFY2(diffFR==diffF, qPrintable(QString("%1 - %2 = %3 (expected: %4)")
			.arg(firstF.toString(), secondF.toString(), diffFR.toString(), diffF.toString())));

		Vec2d diffDR = firstD-secondD;
		QVERIFY2(diffDR==diffD, qPrintable(QString("%1 - %2 = %3 (expected: %4)")
			.arg(firstD.toString(), secondD.toString(), diffDR.toString(), diffD.toString())));

		Vec2i diffIR = firstI-secondI;
		QVERIFY2(diffIR==diffI, qPrintable(QString("%1 - %2 = %3 (expected: %4)")
			.arg(firstI.toString(), secondI.toString(), diffIR.toString(), diffI.toString())));

		Vec2f mulFR = firstF*secondF;
		QVERIFY2(mulFR==mulF, qPrintable(QString("%1 * %2 = %3 (expected: %4)")
			.arg(firstF.toString(), secondF.toString(), mulFR.toString(), mulF.toString())));

		Vec2d mulDR = firstD*secondD;
		QVERIFY2(mulDR==mulD, qPrintable(QString("%1 * %2 = %3 (expected: %4)")
			.arg(firstD.toString(), secondD.toString(), mulDR.toString(), mulD.toString())));

		Vec2i mulIR = firstI*secondI;
		QVERIFY2(mulIR==mulI, qPrintable(QString("%1 * %2 = %3 (expected: %4)")
			.arg(firstI.toString(), secondI.toString(), mulIR.toString(), mulI.toString())));

		Vec2f smFR = firstF*2.f;
		QVERIFY2(smFR==smF, qPrintable(QString("%1 * 2.f = %2 (expected: %3)")
			.arg(firstF.toString(), smFR.toString(), smF.toString())));

		Vec2d smDR = firstD*2.;
		QVERIFY2(smDR==smD, qPrintable(QString("%1 * 2.0 = %2 (expected: %3)")
			.arg(firstD.toString(), smDR.toString(), smD.toString())));

		Vec2i smIR = firstI*2;
		QVERIFY2(smIR==smI, qPrintable(QString("%1 * 2 = %2 (expected: %3)")
			.arg(firstI.toString(), smIR.toString(), smI.toString())));

		vecF *= firstF;
		QVERIFY2(vecF==cwmF, qPrintable(QString("%1 * %1 = %2 (expected: %3)")
			.arg(firstF.toString(), vecF.toString(), cwmF.toString())));

		vecD *= firstD;
		QVERIFY2(vecD==cwmD, qPrintable(QString("%1 * %1 = %2 (expected: %3)")
			.arg(firstD.toString(), vecD.toString(), cwmD.toString())));

		vecI *= firstI;
		QVERIFY2(vecI==cwmI, qPrintable(QString("%1 * %1 = %2 (expected: %3)")
			.arg(firstI.toString(), vecI.toString(), cwmI.toString())));
	}
}

void TestComputations::testVector3Operators()
{
	Vec3f firstF, secondF, vecF, sumF, diffF, smF;
	Vec3d firstD, secondD, vecD, sumD, diffD, smD;
	Vec3i firstI, secondI, vecI, sumI, diffI, smI;
	bool expected, result;
	QVariantList data;
	//           Vec(1)               Vec(2)              Compare   Sum                Diff                   Scal. M.
	data << "0,0,0"		<< "0,0,0"	<< true		<<  "0,0,0"	<<    "0,0,0"	<<   "0,0,0";
	data << "0,0,0"		<< "1,0,0"	<< false	<<  "1,0,0"	<<   "-1,0,0"	<<   "0,0,0";
	data << "0,0,0"		<< "1,1,0"	<< false	<<  "1,1,0"	<< " -1,-1,0"	<<   "0,0,0";
	data << "0,0,0"		<< "1,1,1"	<< false	<<  "1,1,1"	<< "-1,-1,-1"	<<   "0,0,0";
	data << "1,1,1"		<< "0,0,0"	<< false	<<  "1,1,1"	<<    "1,1,1"	<<   "2,2,2";
	data << "1,1,1"		<< "1,1,1"	<< true		<<  "2,2,2"	<<    "0,0,0"	<<   "2,2,2";
	data << "1,-1,0"	<< "1,-1,0"	<< true		<< "2,-2,0"	<<    "0,0,0"	<<  "2,-2,0";
	data << "1,-1,0"	<< "-1,1,0"	<< false	<<  "0,0,0"	<<   "2,-2,0"	<<  "2,-2,0";

	while (data.count() >= 7)
	{
		firstF		= Vec3f(data.takeFirst().toString());
		secondF		= Vec3f(data.takeFirst().toString());
		expected	= data.takeFirst().toBool();
		sumF		= Vec3f(data.takeFirst().toString());
		diffF		= Vec3f(data.takeFirst().toString());
		smF		= Vec3f(data.takeFirst().toString());

		firstD		= firstF.toVec3d();
		secondD		= secondF.toVec3d();
		sumD		= sumF.toVec3d();
		diffD		= diffF.toVec3d();
		smD		= smF.toVec3d();

		firstI.set((int)firstF[0], (int)firstF[1], (int)firstF[2]);
		secondI.set((int)secondF[0], (int)secondF[1], (int)secondF[2]);
		sumI.set((int)sumF[0], (int)sumF[1], (int)sumF[2]);
		diffI.set((int)diffF[0], (int)diffF[1], (int)diffF[2]);
		smI.set((int)smF[0], (int)smF[1], (int)smF[2]);

		result = (firstF==secondF);
		QVERIFY2(result==expected, qPrintable(QString("%1 == %2 (result: %3; expected: %4)")
			.arg(firstF.toString(), secondF.toString()).arg(result).arg(expected)));
		result = (firstD==secondD);
		QVERIFY2(result==expected, qPrintable(QString("%1 == %2 (result: %3; expected: %4)")
			.arg(firstD.toString(), secondD.toString()).arg(result).arg(expected)));
		result = (firstI==secondI);
		QVERIFY2(result==expected, qPrintable(QString("%1 == %2 (result: %3; expected: %4)")
			.arg(firstI.toString(), secondI.toString()).arg(result).arg(expected)));

		result = (firstF!=secondF);
		QVERIFY2(result!=expected, qPrintable(QString("%1 != %2 (result: %3; expected: %4)")
			.arg(firstF.toString(), secondF.toString()).arg(result).arg(expected)));
		result = (firstD!=secondD);
		QVERIFY2(result!=expected, qPrintable(QString("%1 != %2 (result: %3; expected: %4)")
			.arg(firstD.toString(), secondD.toString()).arg(result).arg(expected)));
		result = (firstI!=secondI);
		QVERIFY2(result!=expected, qPrintable(QString("%1 != %2 (result: %3; expected: %4)")
			.arg(firstI.toString(), secondI.toString()).arg(result).arg(expected)));

		Vec3f sumFR = firstF+secondF;
		QVERIFY2(sumFR==sumF, qPrintable(QString("%1 + %2 = %3 (expected: %4)")
			.arg(firstF.toString(), secondF.toString(), sumFR.toString(), sumF.toString())));
		Vec3d sumDR = firstD+secondD;
		QVERIFY2(sumDR==sumD, qPrintable(QString("%1 + %2 = %3 (expected: %4)")
			.arg(firstD.toString(), secondD.toString(), sumDR.toString(), sumD.toString())));
		Vec3i sumIR = firstI+secondI;
		QVERIFY2(sumIR==sumI, qPrintable(QString("%1 + %2 = %3 (expected: %4)")
			.arg(firstI.toString(), secondI.toString(), sumIR.toString(), sumI.toString())));

		Vec3f diffFR = firstF-secondF;
		QVERIFY2(diffFR==diffF, qPrintable(QString("%1 - %2 = %3 (expected: %4)")
			.arg(firstF.toString(), secondF.toString(), diffFR.toString(), diffF.toString())));

		Vec3d diffDR = firstD-secondD;
		QVERIFY2(diffDR==diffD, qPrintable(QString("%1 - %2 = %3 (expected: %4)")
			.arg(firstD.toString(), secondD.toString(), diffDR.toString(), diffD.toString())));

		Vec3i diffIR = firstI-secondI;
		QVERIFY2(diffIR==diffI, qPrintable(QString("%1 - %2 = %3 (expected: %4)")
			.arg(firstI.toString(), secondI.toString(), diffIR.toString(), diffI.toString())));

		Vec3f smFR = firstF*2.f;
		QVERIFY2(smFR==smF, qPrintable(QString("%1 * 2.f = %2 (expected: %3)")
			.arg(firstF.toString(), smFR.toString(), smF.toString())));

		Vec3d smDR = firstD*2.;
		QVERIFY2(smDR==smD, qPrintable(QString("%1 * 2.0 = %2 (expected: %3)")
			.arg(firstD.toString(), smDR.toString(), smD.toString())));

		Vec3i smIR = firstI*2;
		QVERIFY2(smIR==smI, qPrintable(QString("%1 * 2 = %2 (expected: %3)")
			.arg(firstI.toString(), smIR.toString(), smI.toString())));
	}
}

void TestComputations::testIntMod()
{
	QVariantList data;

	data << 1 << 1 << 0;
	data << 1 << 2 << 1;
	data << 1 << 3 << 1;
	data << 2 << 4 << 2;
	data << 2 << 5 << 2;
	data << -2 << 5 << 3;

	while (data.count() >= 3)
	{
		int a = data.takeFirst().toInt();
		int b = data.takeFirst().toInt();
		int eR = data.takeFirst().toInt();
		int r = StelUtils::imod(a, b);

		QVERIFY2(r==eR, qPrintable(QString("%1 mod %2 = %3 (expected %4)").arg(a).arg(b).arg(r).arg(eR)));
	}
}

void TestComputations::testIntDiv()
{
	QVariantList data;

	data << 1 << 1 << 1;
	data << 1 << 2 << 0;
	data << 1 << 3 << 0;
	data << 4 << 2 << 2;
	data << 9 << 5 << 1;
	data << -2 << 5 << 0;
	data << -9 << 5 << -1; // unfortunately...

	while (data.count() >= 3)
	{
		int a = data.takeFirst().toInt();
		int b = data.takeFirst().toInt();
		int eR = data.takeFirst().toInt();
		int r = a/b;

		QVERIFY2(r==eR, qPrintable(QString("Integer division %1 / %2 = %3 (expected %4)")
				.arg(a).arg(b).arg(r).arg(eR)));
	}
}

void TestComputations::testIntFloorDiv()
{
	QVariantList data;

	data << 1 << 1 << 1;
	data << 1 << 2 << 0;
	data << 1 << 3 << 0;
	data << 4 << 2 << 2;
	data << 9 << 5 << 1;
	data << -2 << 5 << -1; // should be next lower integer
	data << -9 << 5 << -2;
	data << -9 << -5 << 1;

	while (data.count() >= 3)
	{
		int a = data.takeFirst().toInt();
		int b = data.takeFirst().toInt();
		int eR = data.takeFirst().toInt();
		int r = StelUtils::intFloorDiv(a, b);

		QVERIFY2(r==eR, qPrintable(QString("Integer division %1 / %2 = %3 (expected %4)")
				.arg(a).arg(b).arg(r).arg(eR)));
	}
}

void TestComputations::testFloatMod()
{
	QVariantList data;

	data << 1 << 1 << 0;
	data << 1 << 2 << 1;
	data << 1 << 3 << 1;
	data << 2 << 4 << 2;
	data << 2 << 5 << 2;
	data << -2 << 5 << 3;

	while (data.count() >= 3)
	{
		float a = data.takeFirst().toFloat();
		float b = data.takeFirst().toFloat();
		float eR = data.takeFirst().toFloat();
		float r = StelUtils::fmodpos(a, b);

		QVERIFY2(qAbs(r-eR)<=1e-5, qPrintable(QString("%1 mod %2 = %3 (expected %4)")
			.arg(QString::number(a, 'f', 2), QString::number(b, 'f', 2), QString::number(r, 'f', 2), QString::number(eR, 'f', 2))));
	}
}

void TestComputations::testDoubleMod()
{
	QVariantList data;

	data << 1 << 1 << 0;
	data << 1 << 2 << 1;
	data << 1 << 3 << 1;
	data << 2 << 4 << 2;
	data << 2 << 5 << 2;
	data << -2 << 5 << 3;

	while (data.count() >= 3)
	{
		double a = data.takeFirst().toDouble();
		double b = data.takeFirst().toDouble();
		double eR = data.takeFirst().toDouble();
		double r = StelUtils::fmodpos(a, b);

		QVERIFY2(qAbs(r-eR)<=1e-5, qPrintable(QString("%1 mod %2 = %3 (expected %4)")
			.arg(QString::number(a, 'f', 2), QString::number(b, 'f', 2), QString::number(r, 'f', 2), QString::number(eR, 'f', 2))));
	}
}

void TestComputations::testExp()
{
	QList<float> data;
	data << -1.f << -0.75f << -0.5f << -0.25f << 0.f << 0.25f << 0.5f << 0.75f << 1.f;
	for (int i = 0; i < data.size(); ++i)
	{
		float v = data.at(i);
		float e = exp(v);
		float r = StelUtils::fastExp(v);
		float err = qAbs(e-r);
		QVERIFY2(err<=1e-2, qPrintable(QString("value: %1 std. exp: %2 fast exp: %3 (error: %4)")
			.arg(QString::number(v, 'f', 5), QString::number(e, 'f', 5), QString::number(r, 'f', 5), QString::number(err, 'f', 5))));
	}
}

void TestComputations::testACos()
{
	QList<float> data;
	data << 0.f << 0.25f << 0.75f << -0.75f << -0.25f << 0.5f << -0.5f;
	for (int i = 0; i < data.size(); ++i)
	{
		float v = data.at(i);
		float e = acos(v);
		float r = StelUtils::fastAcos(v);
		float err = qAbs(e-r);
		QVERIFY2(err<=2e-2, qPrintable(QString("value: %1 std. acos: %2 fast acos: %3 (error: %4)")
			.arg(QString::number(v, 'f', 5), QString::number(e, 'f', 5), QString::number(r, 'f', 5), QString::number(err, 'f', 5))));
	}
}

void TestComputations::testSign()
{
	QVariantList data;
	data << 1 << 1;
	data << -1 << -1;
	data << 10 << 1;
	data << -22 << -1;
	data << 15 << 1;
	data << 0 << 0;
	while (data.count() >= 2)
	{
		int a = data.takeFirst().toInt();
		int e = data.takeFirst().toInt();
		int r = StelUtils::sign(a);

		QVERIFY2(r==e, qPrintable(QString("number: %1 sign: %2 (expected: %3)").arg(a).arg(r).arg(e)));
	}

	data.clear();
	data << 1.0 << 1;
	data << -1.0 << -1;
	data << 10.5 << 1;
	data << -22.1 << -1;
	data << 15.8 << 1;
	data << 0.0 << 0;
	while (data.count() >= 2)
	{
		double a = data.takeFirst().toDouble();
		int e = data.takeFirst().toInt();
		int r = StelUtils::sign(a);

		QVERIFY2(r==e, qPrintable(QString("number: %1 sign: %2 (expected: %3)")
			.arg(QString::number(a, 'f', 1)).arg(r).arg(e)));
	}

	data.clear();
	data << 1.f << 1;
	data << -1.f << -1;
	data << 10.5f << 1;
	data << -22.1f << -1;
	data << 15.8f << 1;
	data << 0.f << 0;
	while (data.count() >= 2)
	{
		float a = data.takeFirst().toFloat();
		int e = data.takeFirst().toInt();
		int r = StelUtils::sign(a);

		QVERIFY2(r==e, qPrintable(QString("number: %1 sign: %2 (expected: %3)")
			.arg(QString::number(a, 'f', 1)).arg(r).arg(e)));
	}
}

void TestComputations::testInterpolation()
{
	QVERIFY(fuzzyEquals(StelUtils::interpolate3(-0.2, 1., 2., 3.), 1.8));
	QVERIFY(fuzzyEquals(StelUtils::interpolate3( 0.2, 1., 2., 3.), 2.2));
	const double i3=StelUtils::interpolate3(4.35/24., .884226, .877366, .870531);  // Meeus 1998, Ex.3a
	QVERIFY2(abs(i3-.8761253) < 0.000001,
		 qPrintable(QString("Interpol3 returned %1").arg(QString::number(i3, 'g', 7))));
	const double i5=StelUtils::interpolate5(0.2777778,                             // Meeus 1998, Ex.3e
			StelUtils::dmsToRad(0, 54, 36.125),
			StelUtils::dmsToRad(0, 54, 24.606),
			StelUtils::dmsToRad(0, 54, 15.486),
			StelUtils::dmsToRad(0, 54, 08.694),
			StelUtils::dmsToRad(0, 54, 04.133));
	double res5=StelUtils::dmsToRad(0, 54, 13.369);
	QVERIFY2(abs(i5-res5)<0.000001,
		 qPrintable(QString("Interpol5 returned %1, not %2").arg(QString::number(i5, 'g', 7).arg(QString::number(res5, 'g', 7)))));
}

void TestComputations::testIsWithin()
{
	int a = 1, b = 5, c = 8;
	QVERIFY2( StelUtils::isWithin(b, a, c), qPrintable(QString("%1 in [%2, %3]").arg(b).arg(a).arg(c)));
	QVERIFY2(!StelUtils::isWithin(a, b, c), qPrintable(QString("%1 in [%2, %3]").arg(a).arg(b).arg(c)));
	QVERIFY2(!StelUtils::isWithin(c, a, b), qPrintable(QString("%1 in [%2, %3]").arg(c).arg(a).arg(b)));
	QVERIFY2( StelUtils::isWithin(b, b, c), qPrintable(QString("%1 in [%2, %3]").arg(b).arg(b).arg(c)));

	float i = 1.f, j = 5.f, k = 8.f;
	QVERIFY2( StelUtils::isWithin(j, i, k), qPrintable(QString("%1 in [%2, %3]")
		.arg(QString::number(j,'f',3), QString::number(i,'f',3), QString::number(k,'f',3))));
	QVERIFY2(!StelUtils::isWithin(i, j, k), qPrintable(QString("%1 in [%2, %3]")
		.arg(QString::number(i,'f',3), QString::number(j,'f',3), QString::number(k,'f',3))));
	QVERIFY2(!StelUtils::isWithin(k, i, j), qPrintable(QString("%1 in [%2, %3]")
		.arg(QString::number(k,'f',3), QString::number(i,'f',3), QString::number(j,'f',3))));
	QVERIFY2( StelUtils::isWithin(j, j, k), qPrintable(QString("%1 in [%2, %3]")
		.arg(QString::number(j,'f',3), QString::number(j,'f',3), QString::number(k,'f',3))));
}
