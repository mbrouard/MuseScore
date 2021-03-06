//=============================================================================
//  MuseScore
//  Music Composition & Notation
//  $Id:$
//
//  Copyright (C) 2012 Werner Schweer
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License version 2
//  as published by the Free Software Foundation and appearing in
//  the file LICENCE.GPL
//=============================================================================

#include <QtTest/QtTest>

#include "libmscore/text.h"
#include "libmscore/score.h"
#include "libmscore/sym.h"
#include "mtest/testutils.h"

using namespace Ms;

//---------------------------------------------------------
//   TestNote
//---------------------------------------------------------

class TestText : public QObject, public MTest
      {
      Q_OBJECT

   private slots:
      void initTestCase();
      void testText();
      void testSpecialSymbols();
      void testTextProperties();
      void testCompatibility();
      };

//---------------------------------------------------------
//   initTestCase
//---------------------------------------------------------

void TestText::initTestCase()
      {
      initMTest();
      }

//---------------------------------------------------------
///   testText
//---------------------------------------------------------

void TestText::testText()
      {
      Text* text = new Text(score);
      text->setTextStyle(score->textStyle(TextStyleType::DYNAMICS));

      text->setEditMode(true);
      text->layout();

      text->moveCursorToEnd();
      text->insertText("a");
      text->endEdit();
      QCOMPARE(text->text(), QString("a"));

      text->setEditMode(true);
      text->moveCursorToEnd();
      text->insertText("bc");
      text->endEdit();
      QCOMPARE(text->text(), QString("abc"));

      text->setEditMode(true);
      text->moveCursorToEnd();
      text->insertText("d");
      text->insertText("e");
      text->endEdit();
      QCOMPARE(text->text(), QString("abcde"));

      text->setEditMode(true);
      text->moveCursorToStart();
      text->insertText("1");
      text->endEdit();
      QCOMPARE(text->text(), QString("1abcde"));

      text->setEditMode(true);
      text->moveCursorToStart();
      text->insertText("0");
      text->endEdit();
      QCOMPARE(text->text(), QString("01abcde"));

      text->setEditMode(true);
      text->moveCursorToStart();
      text->movePosition(QTextCursor::Right);
      text->movePosition(QTextCursor::Right);
      text->insertText("2");
      text->endEdit();
      QCOMPARE(text->text(), QString("012abcde"));

      text->setEditMode(true);
      text->moveCursorToEnd();
      text->movePosition(QTextCursor::Left);
      text->movePosition(QTextCursor::Left);
      text->movePosition(QTextCursor::Left);
      text->movePosition(QTextCursor::Left);
      text->movePosition(QTextCursor::Left);
      text->insertText("3");
      text->endEdit();
      QCOMPARE(text->text(), QString("0123abcde"));

      text->setEditMode(true);
      text->moveCursorToEnd();
      text->insertSym(SymId::segno);
      text->endEdit();
      QCOMPARE(text->text(), QString("0123abcde<sym>segno</sym>"));

      text->setEditMode(true);
      text->movePosition(QTextCursor::Left);
      text->insertText("#");
      text->endEdit();
      QCOMPARE(text->text(), QString("0123abcde#<sym>segno</sym>"));
      }

//---------------------------------------------------------
///   testSpecialSymbols
//---------------------------------------------------------

void TestText::testSpecialSymbols()
      {
      Text* text = new Text(score);
      text->setTextStyle(score->textStyle(TextStyleType::DYNAMICS));

      text->setEditMode(true);
      text->layout();

      text->moveCursorToEnd();
      text->insertText("<");
      text->endEdit();
      QCOMPARE(text->text(), QString("&lt;"));

      text->selectAll();
      text->deleteSelectedText();
      text->insertText("&");
      text->endEdit();
      QCOMPARE(text->text(), QString("&amp;"));

      text->selectAll();
      text->deleteSelectedText();
      text->insertText(">");
      text->endEdit();
      QCOMPARE(text->text(), QString("&gt;"));

      text->selectAll();
      text->deleteSelectedText();
      text->insertText("\"");
      text->endEdit();
      QCOMPARE(text->text(), QString("&quot;"));

      text->selectAll();
      text->deleteSelectedText();
      text->insertText("&gt;");
      text->endEdit();
      QCOMPARE(text->text(), QString("&amp;gt;"));
      }

//---------------------------------------------------------
///   testTextProperties
//---------------------------------------------------------

void TestText::testTextProperties()
      {
      Text* text = new Text(score);
      text->setTextStyle(score->textStyle(TextStyleType::DYNAMICS));

      text->setEditMode(true);
      text->layout();

      text->moveCursorToEnd();
      text->insertText("ILoveMuseScore");
      text->endEdit();
      QCOMPARE(text->text(), QString("ILoveMuseScore"));

      //select Love and make it bold
      text->setEditMode(true);
      text->moveCursorToStart();
      text->movePosition(QTextCursor::Right);
      text->movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, 4);

      text->setFormat(FormatId::Bold , true);
      text->endEdit();
      QCOMPARE(text->text(), QString("I<b>Love</b>MuseScore"));

      //select Love and unbold it
      text->setEditMode(true);
      text->moveCursorToStart();
      text->movePosition(QTextCursor::Right);
      text->movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, 4);

      text->setFormat(FormatId::Bold , false);
      text->endEdit();
      QCOMPARE(text->text(), QString("ILoveMuseScore"));

      //select Love and make it bold again
      text->setEditMode(true);
      text->moveCursorToStart();
      text->movePosition(QTextCursor::Right);
      text->movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, 4);

      text->setFormat(FormatId::Bold , true);
      text->endEdit();
      QCOMPARE(text->text(), QString("I<b>Love</b>MuseScore"));

      //select veMu and make it bold
      text->setEditMode(true);
      text->moveCursorToStart();
      text->movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, 3);
      text->movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, 4);

      text->setFormat(FormatId::Bold , true);
      text->endEdit();
      QCOMPARE(text->text(), QString("I<b>LoveMu</b>seScore"));

      //select Mu and make it nonbold
      text->setEditMode(true);
      text->moveCursorToStart();
      text->movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, 5);
      text->movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, 2);

      text->setFormat(FormatId::Bold , false);
      text->endEdit();
      QCOMPARE(text->text(), QString("I<b>Love</b>MuseScore"));

      //make veMuse italic
      text->setEditMode(true);
      text->moveCursorToStart();
      QVERIFY(text->movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, 3));
      QVERIFY(text->movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, 6));

      text->setFormat(FormatId::Italic , true);
      text->endEdit();
      QCOMPARE(text->text(), QString("I<b>Lo<i>ve</i></b><i>Muse</i>Score"));

      }


//---------------------------------------------------------
///   testCompatibility
//---------------------------------------------------------

void TestText::testCompatibility() {
      Text* text = new Text(score);
      //bold
      const QString sb("<html><head><meta name=\"qrichtext\" content=\"1\" /><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" /><style type=\"text/css\">"
"p, li { white-space: pre-wrap; }"
"</style></head><body style=\" font-family:'Times New Roman'; font-size:10.0006pt; font-weight:400; font-style:normal;\">"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">bold</span></p></body></html>");
      QCOMPARE(text->convertFromHtml(sb), QString("<font face=\"Times New Roman\"/><b>bold</b>"));

      //italic
      const QString si("<html><head><meta name=\"qrichtext\" content=\"1\" /><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" /><style type=\"text/css\">"
"p, li { white-space: pre-wrap; }"
"</style></head><body style=\" font-family:'Times New Roman'; font-size:10.0006pt; font-weight:400; font-style:normal;\">"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-style:italic;\">italic</span></p></body></html>");
      QCOMPARE(text->convertFromHtml(si), QString("<font face=\"Times New Roman\"/><i>italic</i>"));

      //underline
      const QString su("<html><head><meta name=\"qrichtext\" content=\"1\" /><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" /><style type=\"text/css\">"
"p, li { white-space: pre-wrap; }"
"</style></head><body style=\" font-family:'Times New Roman'; font-size:10.0006pt; font-weight:400; font-style:normal;\">"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" text-decoration: underline;\">underline</span></p></body></html>");
      QCOMPARE(text->convertFromHtml(su), QString("<font face=\"Times New Roman\"/><u>underline</u>"));

      //bold italic underline
      const QString sbiu("<html><head><meta name=\"qrichtext\" content=\"1\" /><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" /><style type=\"text/css\">"
"p, li { white-space: pre-wrap; }"
"</style></head><body style=\" font-family:'Times New Roman'; font-size:10.0006pt; font-weight:400; font-style:normal;\">"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600; font-style:italic; text-decoration: underline;\">bolditalicunderline</span></p></body></html>");
      QCOMPARE(text->convertFromHtml(sbiu), QString("<font face=\"Times New Roman\"/><b><i><u>bolditalicunderline</u></i></b>"));

      const QString sbiu2("<html><head><meta name=\"qrichtext\" content=\"1\" /><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" /><style type=\"text/css\">"
"p, li { white-space: pre-wrap; }"
"</style></head><body style=\" font-family:'Times New Roman'; font-size:10.0006pt; font-weight:400; font-style:normal;\">"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">bold</span><span style=\" font-style:italic;\">italic</span><span style=\" text-decoration: underline;\">underline</span></p></body></html>");
      QCOMPARE(text->convertFromHtml(sbiu2), QString("<font face=\"Times New Roman\"/><b>bold</b><i>italic</i><u>underline</u>"));

      const QString sbi("<html><head><meta name=\"qrichtext\" content=\"1\" /><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" /><style type=\"text/css\">"
"p, li { white-space: pre-wrap; }"
"</style></head><body style=\" font-family:'Times New Roman'; font-size:10.0006pt; font-weight:400; font-style:normal;\">"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">bo</span><span style=\" font-weight:600; font-style:italic; text-decoration: underline;\">ldit</span>alic</p></body></html>");
      QCOMPARE(text->convertFromHtml(sbi), QString("<font face=\"Times New Roman\"/><b>bo</b><b><i><u>ldit</u></i></b>alic"));

      const QString sescape("<html><head><meta name=\"qrichtext\" content=\"1\" /><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" /><style type=\"text/css\">"
"p, li { white-space: pre-wrap; }"
"</style></head><body style=\" font-family:'Times New Roman'; font-size:10.0006pt; font-weight:400; font-style:normal;\">"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">test&amp;&lt;&gt;&quot;'</p></body></html>");
      QCOMPARE(text->convertFromHtml(sescape), QString("<font face=\"Times New Roman\"/>test&amp;&lt;&gt;&quot;'"));


}


QTEST_MAIN(TestText)

#include "tst_text.moc"

