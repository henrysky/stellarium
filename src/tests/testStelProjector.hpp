/*
 * Stellarium
 * Copyright (C) 2009 Matthew Gates
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

#ifndef TESTSTELPROJECTOR_HPP
#define TESTSTELPROJECTOR_HPP

#include <QObject>
#include <QtTest>

class TestStelProjector : public QObject
{
	Q_OBJECT
private slots:
	void testStelProjectorPerspective();
	void testStelProjectorEqualArea();
	void testStelProjectorStereographic();
	void testStelProjectorFisheye();
	void testStelProjectorHammer();
	void testStelProjectorMollweide();
	void testStelProjectorCylinder();
	void testStelProjectorMercator();
	void testStelProjectorOrthographic();
	void testStelProjectorSinusoidal();
	void testStelProjectorMiller();
};

#endif // _TESTSTELPROJECTOR_HPP
