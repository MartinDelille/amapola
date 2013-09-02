#include "mysubtitlestable.h"
#include "mysubtitles.h"
#include <QTime>
#include <QTextDocumentFragment>

#define SUB_NUM_COL 0
#define SUB_START_TIME_COL 1
#define SUB_END_TIME_COL 2
#define SUB_DURATION_COL 3
#define SUB_TEXT_COL 4

#define SUB_NBR_OF_LINES_COL 5

#define SUB_LINE1_COL 6
#define SUB_VPOSITION1_COL 7
#define SUB_VALIGN1_COL 8
#define SUB_HPOSITION1_COL 9
#define SUB_HALIGN1_COL 10
#define SUB_DIRECTION1_COL 11
#define SUB_FONT_ID1_COL 12
#define SUB_FONT_COLOR1_COL 13
#define SUB_FONT_EFFECT1_COL 14
#define SUB_FONT_EFFECTCOLOR1_COL 15
#define SUB_FONT_ITALIC1_COL 16
#define SUB_FONT_UNDERLINED1_COL 17
#define SUB_FONT_SCRIPT1_COL 18
#define SUB_FONT_SIZE1_COL 19

#define SUB_LINE2_COL 20
#define SUB_VPOSITION2_COL 21
#define SUB_VALIGN2_COL 22
#define SUB_HPOSITION2_COL 23
#define SUB_HALIGN2_COL 24
#define SUB_DIRECTION2_COL 25
#define SUB_FONT_ID2_COL 26
#define SUB_FONT_COLOR2_COL 27
#define SUB_FONT_EFFECT2_COL 28
#define SUB_FONT_EFFECTCOLOR2_COL 29
#define SUB_FONT_ITALIC2_COL 30
#define SUB_FONT_UNDERLINED2_COL 31
#define SUB_FONT_SCRIPT2_COL 32
#define SUB_FONT_SIZE2_COL 33

#define SUB_DEFAULT_DURATION_MS 1000

MySubtitlesTable::MySubtitlesTable(QWidget *parent) :
    QTableWidget(parent)
{
    this->setColumnHidden(5, true);
    // Init the vector
    mPositionMsToStIndex.resize(1);
    mPositionMsToStIndex.fill(-1);

    // Init the variables
    mVideoPositionMs = 0;
    mPreviousIndex = 0;

    mStCount = 0;

    mSubLoadding = false;

    // Init the connections
    connect(this, SIGNAL(itemSelectionChanged()), this, SLOT(updateSelectedItem()));
    connect(this, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(updateItem(QTableWidgetItem*)));

}

// Init all the table items with an empty string
void MySubtitlesTable::initStTable (qint32 numberOfRow) {

    this->setEnabled(false);

    mStCount = 0;

    if ( numberOfRow == 0 ) {
        this->setRowCount(100);
    }
    else {
        this->setRowCount(numberOfRow);
    }

    for ( qint32 i = 0; i < this->columnCount(); i++ ) {
        for ( qint32 j = 0; j < this->rowCount(); j++) {

            QTableWidgetItem* new_item = new QTableWidgetItem;

            // The start time items are init with "New Entry" string to able sorting
            if ( i == SUB_START_TIME_COL ) {
                new_item->setText("New Entry");
                new_item->setTextAlignment(Qt::AlignCenter);
            }
            else if ( i == SUB_TEXT_COL ){
                new_item->setText("");
                new_item->setTextAlignment(Qt::AlignLeft);
            }
            else if ( i == SUB_NBR_OF_LINES_COL ) {
                new_item->setText("0");
                new_item->setTextAlignment(Qt::AlignCenter);
            }
            else {
                new_item->setText("");
                new_item->setTextAlignment(Qt::AlignCenter);
            }
            this->setItem(j, i, new_item);
        }
    }
}

void MySubtitlesTable::loadSubtitles(QList<MySubtitles> subtitlesList) {

    QTableWidgetItem* start_time_item;
    QTableWidgetItem* end_time_item;
    QTableWidgetItem* text_item;

    QTableWidgetItem* line1_item;
    QTableWidgetItem* vposition1_item;
    QTableWidgetItem* valign1_item;
    QTableWidgetItem* hposition1_item;
    QTableWidgetItem* halign1_item;
    QTableWidgetItem* direction1_item;
    QTableWidgetItem* font_id1_item;
    QTableWidgetItem* font_color1_item;
    QTableWidgetItem* font_effect1_item;
    QTableWidgetItem* font_effectcolor1_item;
    QTableWidgetItem* font_italic1_item;
    QTableWidgetItem* font_underlined1_item;
    QTableWidgetItem* font_script1_item;
    QTableWidgetItem* font_size1_item;
    QTableWidgetItem* line2_item;
    QTableWidgetItem* vposition2_item;
    QTableWidgetItem* valign2_item;
    QTableWidgetItem* hposition2_item;
    QTableWidgetItem* halign2_item;
    QTableWidgetItem* direction2_item;
    QTableWidgetItem* font_id2_item;
    QTableWidgetItem* font_color2_item;
    QTableWidgetItem* font_effect2_item;
    QTableWidgetItem* font_effectcolor2_item;
    QTableWidgetItem* font_italic2_item;
    QTableWidgetItem* font_underlined2_item;
    QTableWidgetItem* font_script2_item;
    QTableWidgetItem* font_size2_item;

    if ( this->isEnabled() == true ) {

        initStTable(subtitlesList.size() + 20);

        mSubLoadding = true;

        for ( qint32 i = 0; i < subtitlesList.size(); i++ ) {

            start_time_item = this->item(mStCount, SUB_START_TIME_COL);
            start_time_item->setText( subtitlesList[i].startTime() );

            end_time_item = this->item(mStCount, SUB_END_TIME_COL);
            end_time_item->setText( subtitlesList[i].endTime() );

            text_item = this->item(mStCount, SUB_TEXT_COL);

            QString text = "";
            QList<TextLine> text_list = subtitlesList[i].text();
            for ( qint32 j = 0; j < text_list.size(); j++ ) {

                if ( !text.isEmpty() ) {
                    text += "\n";
                }
                TextLine text_line = text_list.at(j);
                text += text_line.Line();

                if ( j == 0 ) {
                    this->item(mStCount, SUB_NBR_OF_LINES_COL)->setText("1");
                    line1_item  = this->item(mStCount, SUB_LINE1_COL);
                    line1_item->setText( text_line.Line() );
                    vposition1_item  = this->item(mStCount, SUB_VPOSITION1_COL);
                    vposition1_item->setText( text_line.textVPosition() );
                    valign1_item  = this->item(mStCount, SUB_VALIGN1_COL);
                    valign1_item->setText( text_line.textVAlign() );
                    hposition1_item  = this->item(mStCount, SUB_HPOSITION1_COL);
                    hposition1_item->setText( text_line.textHPosition() );
                    halign1_item  = this->item(mStCount, SUB_HALIGN1_COL);
                    halign1_item->setText( text_line.textHAlign() );
                    direction1_item  = this->item(mStCount, SUB_DIRECTION1_COL);
                    direction1_item->setText( text_line.textDirection() );
                    font_id1_item  = this->item(mStCount, SUB_FONT_ID1_COL);
                    font_id1_item->setText( text_line.Font().fontId() );
                    font_color1_item  = this->item(mStCount, SUB_FONT_COLOR1_COL);
                    font_color1_item->setText( text_line.Font().fontColor() );
                    font_effect1_item  = this->item(mStCount, SUB_FONT_EFFECT1_COL);
                    font_effect1_item->setText( text_line.Font().fontEffect() );
                    font_effectcolor1_item  = this->item(mStCount, SUB_FONT_EFFECTCOLOR1_COL);
                    font_effectcolor1_item->setText( text_line.Font().fontEffectColor() );
                    font_italic1_item  = this->item(mStCount, SUB_FONT_ITALIC1_COL);
                    font_italic1_item->setText( text_line.Font().fontItalic() );
                    font_underlined1_item  = this->item(mStCount, SUB_FONT_UNDERLINED1_COL);
                    font_underlined1_item->setText( text_line.Font().fontUnderlined() );
                    font_script1_item  = this->item(mStCount, SUB_FONT_SCRIPT1_COL);
                    font_script1_item->setText( text_line.Font().fontScript() );
                    font_size1_item  = this->item(mStCount, SUB_FONT_SIZE1_COL);
                    font_size1_item->setText( text_line.Font().fontSize() );
                }
                else if ( j == 1 ) {
                    this->item(mStCount, SUB_NBR_OF_LINES_COL)->setText("2");
                    line2_item  = this->item(mStCount, SUB_LINE2_COL);
                    line2_item->setText( text_line.Line() );
                    vposition2_item  = this->item(mStCount, SUB_VPOSITION2_COL);
                    vposition2_item->setText( text_line.textVPosition() );
                    valign2_item  = this->item(mStCount, SUB_VALIGN2_COL);
                    valign2_item->setText( text_line.textVAlign() );
                    hposition2_item  = this->item(mStCount, SUB_HPOSITION2_COL);
                    hposition2_item->setText( text_line.textHPosition() );
                    halign2_item  = this->item(mStCount, SUB_HALIGN2_COL);
                    halign2_item->setText( text_line.textHAlign() );
                    direction2_item  = this->item(mStCount, SUB_DIRECTION2_COL);
                    direction2_item->setText( text_line.textDirection() );
                    font_id2_item  = this->item(mStCount, SUB_FONT_ID2_COL);
                    font_id2_item->setText( text_line.Font().fontId() );
                    font_color2_item  = this->item(mStCount, SUB_FONT_COLOR2_COL);
                    font_color2_item->setText( text_line.Font().fontColor() );
                    font_effect2_item  = this->item(mStCount, SUB_FONT_EFFECT2_COL);
                    font_effect2_item->setText( text_line.Font().fontEffect() );
                    font_effectcolor2_item  = this->item(mStCount, SUB_FONT_EFFECTCOLOR2_COL);
                    font_effectcolor2_item->setText( text_line.Font().fontEffectColor() );
                    font_italic2_item  = this->item(mStCount, SUB_FONT_ITALIC2_COL);
                    font_italic2_item->setText( text_line.Font().fontItalic() );
                    font_underlined2_item  = this->item(mStCount, SUB_FONT_UNDERLINED2_COL);
                    font_underlined2_item->setText( text_line.Font().fontUnderlined() );
                    font_script2_item  = this->item(mStCount, SUB_FONT_SCRIPT2_COL);
                    font_script2_item->setText( text_line.Font().fontScript() );
                    font_size2_item  = this->item(mStCount, SUB_FONT_SIZE2_COL);
                    font_size2_item->setText( text_line.Font().fontSize() );
                }
            }

            text_item->setText(text);

            mStCount ++;
        }
        mSubLoadding = false;
        this->setEnabled(true);

    }
}

QList<MySubtitles> MySubtitlesTable::saveSubtitles() {

    QList<MySubtitles> subtitles_list;

    for ( qint32 i = 0; i < mStCount; i++) {

//        MySubtitles subtitle;
//        TextLine text_line;
//        TextFont text_font;

//        subtitle.setStartTime( this->item(i, SUB_START_TIME_COL)->text() );
//        subtitle.setEndTime( this->item(i, SUB_END_TIME_COL)->text() );
//        //subtitle.setText( this->item(i, SUB_TEXT_COL)->text() );

        subtitles_list.append(this->fillSubInfos(i));
    }
    return subtitles_list;
}

// Insert or modify a subtitle in the table
void MySubtitlesTable::insertNewSub(MySubtitles newSubtitle) {

    QTableWidgetItem* start_time_item;
    QTableWidgetItem* end_time_item;
    QTime start_time_HMS;
    QTime end_time_HMS;
    qint32 start_time_ms;
    qint32 end_time_ms;
    QTime time_base(0, 0, 0, 0);

    if ( !newSubtitle.text().isEmpty() ) {

        TextLine text_line = newSubtitle.text().first();

        if ( ( text_line.Line() != "" ) || (newSubtitle.text().size() > 1) ) {

            start_time_ms = mVideoPositionMs;
            end_time_ms = mVideoPositionMs + SUB_DEFAULT_DURATION_MS;

            if ( mPositionMsToStIndex[start_time_ms] == -1) {

                // Retrieive the first empty item, set the start/end time and text
                start_time_HMS = time_base.addMSecs(start_time_ms);
                start_time_item = this->item(mStCount, SUB_START_TIME_COL);
                start_time_item->setText(start_time_HMS.toString("hh:mm:ss.zzz"));

                end_time_HMS = time_base.addMSecs(end_time_ms);
                end_time_item = this->item(mStCount, SUB_END_TIME_COL);
                end_time_item->setText(end_time_HMS.toString("hh:mm:ss.zzz"));

                this->updateText(newSubtitle.text());
                this->updateDatas(newSubtitle);
                //this->item(mPositionMsToStIndex[start_time_ms], SUB_TEXT_COL)->setText(stText);
            }
        }
    }
}

bool MySubtitlesTable::isNewEntry() {

    qint32 row_index = mPositionMsToStIndex[mVideoPositionMs];

    // The subtitle exist
    if (row_index != -1) {
        return false;
    }
    else { // The subtitle not exist
        return true;
    }
}

void MySubtitlesTable::updateText(QList<TextLine> textLines) {

    qint32 row_index = mPositionMsToStIndex[mVideoPositionMs];
    QString plain_text;
    TextLine text_line;

    for ( qint32 i = 0; i < textLines.count(); i++ ) {

        text_line = textLines.at(i);

        if ( i == 0 ) {

            this->item(row_index, SUB_NBR_OF_LINES_COL)->setText("1");
            this->item(row_index, SUB_LINE1_COL)->setText(text_line.Line());
            plain_text = text_line.Line();

            if ( textLines.count() == 1 ) {
                this->item(row_index, SUB_LINE2_COL)->setText("");
            }
        }
        else if ( i == 1 ) {

            this->item(row_index, SUB_NBR_OF_LINES_COL)->setText("2");
            this->item(row_index, SUB_LINE2_COL)->setText(text_line.Line());
            plain_text += "\n";
            plain_text += text_line.Line();
        }
    }
    this->item(row_index, SUB_TEXT_COL)->setText(plain_text);
}

void MySubtitlesTable::updateDatas(MySubtitles subtitle) {

    qint32 index;

    QTableWidgetItem* vposition1_item;
    QTableWidgetItem* valign1_item;
    QTableWidgetItem* hposition1_item;
    QTableWidgetItem* halign1_item;
    QTableWidgetItem* direction1_item;
    QTableWidgetItem* font_id1_item;
    QTableWidgetItem* font_color1_item;
    QTableWidgetItem* font_effect1_item;
    QTableWidgetItem* font_effectcolor1_item;
    QTableWidgetItem* font_italic1_item;
    QTableWidgetItem* font_underlined1_item;
    QTableWidgetItem* font_script1_item;
    QTableWidgetItem* font_size1_item;
    QTableWidgetItem* vposition2_item;
    QTableWidgetItem* valign2_item;
    QTableWidgetItem* hposition2_item;
    QTableWidgetItem* halign2_item;
    QTableWidgetItem* direction2_item;
    QTableWidgetItem* font_id2_item;
    QTableWidgetItem* font_color2_item;
    QTableWidgetItem* font_effect2_item;
    QTableWidgetItem* font_effectcolor2_item;
    QTableWidgetItem* font_italic2_item;
    QTableWidgetItem* font_underlined2_item;
    QTableWidgetItem* font_script2_item;
    QTableWidgetItem* font_size2_item;

    if ( mPositionMsToStIndex[mVideoPositionMs] != -1) {

        index = mPositionMsToStIndex[mVideoPositionMs];

        for ( qint32 i = 0; i < subtitle.text().size(); i++) {

            TextLine text_line = subtitle.text().at(i);

            if ( i == 0 ) {

                for ( qint32 j = SUB_VPOSITION2_COL; j <= SUB_FONT_SIZE2_COL; j++ ) {
                    this->item(index, j)->setText("");
                }

                vposition1_item  = this->item(index, SUB_VPOSITION1_COL);
                vposition1_item->setText( text_line.textVPosition() );
                valign1_item  = this->item(index, SUB_VALIGN1_COL);
                valign1_item->setText( text_line.textVAlign() );
                hposition1_item  = this->item(index, SUB_HPOSITION1_COL);
                hposition1_item->setText( text_line.textHPosition() );
                halign1_item  = this->item(index, SUB_HALIGN1_COL);
                halign1_item->setText( text_line.textHAlign() );
                direction1_item  = this->item(index, SUB_DIRECTION1_COL);
                direction1_item->setText( text_line.textDirection() );
                font_id1_item  = this->item(index, SUB_FONT_ID1_COL);
                font_id1_item->setText( text_line.Font().fontId() );
                font_color1_item  = this->item(index, SUB_FONT_COLOR1_COL);
                font_color1_item->setText( text_line.Font().fontColor() );
                font_effect1_item  = this->item(index, SUB_FONT_EFFECT1_COL);
                font_effect1_item->setText( text_line.Font().fontEffect() );
                font_effectcolor1_item  = this->item(index, SUB_FONT_EFFECTCOLOR1_COL);
                font_effectcolor1_item->setText( text_line.Font().fontEffectColor() );
                font_italic1_item  = this->item(index, SUB_FONT_ITALIC1_COL);
                font_italic1_item->setText( text_line.Font().fontItalic() );
                font_underlined1_item  = this->item(index, SUB_FONT_UNDERLINED1_COL);
                font_underlined1_item->setText( text_line.Font().fontUnderlined() );
                font_script1_item  = this->item(index, SUB_FONT_SCRIPT1_COL);
                font_script1_item->setText( text_line.Font().fontScript() );
                font_size1_item  = this->item(index, SUB_FONT_SIZE1_COL);
                font_size1_item->setText( text_line.Font().fontSize() );
            }
            else if ( i == 1 ) {
                vposition2_item  = this->item(index, SUB_VPOSITION2_COL);
                vposition2_item->setText( text_line.textVPosition() );
                valign2_item  = this->item(index, SUB_VALIGN2_COL);
                valign2_item->setText( text_line.textVAlign() );
                hposition2_item  = this->item(index, SUB_HPOSITION2_COL);
                hposition2_item->setText( text_line.textHPosition() );
                halign2_item  = this->item(index, SUB_HALIGN2_COL);
                halign2_item->setText( text_line.textHAlign() );
                direction2_item  = this->item(index, SUB_DIRECTION2_COL);
                direction2_item->setText( text_line.textDirection() );
                font_id2_item  = this->item(index, SUB_FONT_ID2_COL);
                font_id2_item->setText( text_line.Font().fontId() );
                font_color2_item  = this->item(index, SUB_FONT_COLOR2_COL);
                font_color2_item->setText( text_line.Font().fontColor() );
                font_effect2_item  = this->item(index, SUB_FONT_EFFECT2_COL);
                font_effect2_item->setText( text_line.Font().fontEffect() );
                font_effectcolor2_item  = this->item(index, SUB_FONT_EFFECTCOLOR2_COL);
                font_effectcolor2_item->setText( text_line.Font().fontEffectColor() );
                font_italic2_item  = this->item(index, SUB_FONT_ITALIC2_COL);
                font_italic2_item->setText( text_line.Font().fontItalic() );
                font_underlined2_item  = this->item(index, SUB_FONT_UNDERLINED2_COL);
                font_underlined2_item->setText( text_line.Font().fontUnderlined() );
                font_script2_item  = this->item(index, SUB_FONT_SCRIPT2_COL);
                font_script2_item->setText( text_line.Font().fontScript() );
                font_size2_item  = this->item(index, SUB_FONT_SIZE2_COL);
                font_size2_item->setText( text_line.Font().fontSize() );
            }
        }
    }
}

// Re-direct the items modifications
void MySubtitlesTable::updateItem(QTableWidgetItem *item) {

    if ( ( item->column() == SUB_START_TIME_COL ) || ( item->column() == SUB_END_TIME_COL ) ) {

        // Bypass during the table init
        if ( ( item->text() != "") && ( item->text() != "New Entry" ) ) {

            if ( !updateStTime(item) ) {

                this->editItem(item);
            }
        }
    }
}

// Manage time change of an start/end time item
bool MySubtitlesTable::updateStTime(QTableWidgetItem* time_item) {

    QTableWidgetItem* start_time_item;
    QTableWidgetItem* end_time_item;
    QTableWidgetItem* duration_time_item;
    QTime time_base(0, 0, 0, 0);
    QTime start_time_HMS;
    QTime end_time_HMS;
    QTime duration_time_HMS;
    qint32 start_time_ms;
    qint32 end_time_ms;
    qint32 duration_time_ms;
    qint32 row_index;
    qint32 new_row_index;

    bool compute_duration = false;

    // Retrieive the modified row index
    row_index = time_item->row();

    if ( time_item->column() == SUB_START_TIME_COL ) {

        start_time_item = time_item;
        end_time_item = this->item(row_index, SUB_END_TIME_COL);

        start_time_HMS = QTime::fromString(start_time_item->text(), "hh:mm:ss.zzz");

        // Check if the start time entry is valid
        if ( start_time_HMS.isValid() ) {

            end_time_HMS = QTime::fromString(end_time_item->text(), "hh:mm:ss.zzz");

            // Check if the end time entry is valid
            if ( end_time_HMS.isValid() ) {
                // Time is valid, continue to manage the new entry
                compute_duration = true;
            }
        }
        else { // The new start entry is not a valid entry
            return false;
        }
    }
    else if ( time_item->column() == SUB_END_TIME_COL ) {

        end_time_item = time_item;
        start_time_item = this->item(row_index, SUB_START_TIME_COL);

        end_time_HMS = QTime::fromString(end_time_item->text(), "hh:mm:ss.zzz");

        // Check if the end time entry is valid
        if ( end_time_HMS.isValid() ) {

            start_time_HMS = QTime::fromString(start_time_item->text(), "hh:mm:ss.zzz");

            // Check if the start time entry is valid
            if ( start_time_HMS.isValid() ) {
                // Time is valid, continue to manage the new entry
                compute_duration = true;
            }
        }
        else { // The new end entry is not a valid entry
            return false;
        }
    }

    if ( compute_duration ) {

        qint32 first_index;
        qint32 last_index;

        // Compute the duration
        start_time_ms = qAbs( start_time_HMS.msecsTo(time_base) );
        end_time_ms = qAbs( end_time_HMS.msecsTo(time_base) );

        duration_time_ms = end_time_ms - start_time_ms;
        duration_time_HMS = time_base.addMSecs(duration_time_ms);

        duration_time_item = this->item(row_index, SUB_DURATION_COL);
        duration_time_item->setText(duration_time_HMS.toString("hh:mm:ss.zzz"));

        // When loading a subtitle file, it's not necessary to sort items
        if ( mSubLoadding == true ) {

            // Just insert the new index in the lookup table
            for ( qint32 i = start_time_ms; i <= end_time_ms; i++ ) {

                mPositionMsToStIndex[i] = mStCount;
            }

            // Display the new subtitle index
            this->item(mStCount, SUB_NUM_COL)->setText(QString::number(mStCount + 1));
        }
        else { // A subtitle is incerted or modified by user

            // If it's new entry
            if ( this->item(row_index, SUB_NUM_COL)->text() == "" ) {

                // Increment the subtitles items counter
                mStCount ++;
                // Sort the st table to add the new subtitle at good place
                this->sortItems(SUB_START_TIME_COL, Qt::AscendingOrder);

                new_row_index = 0;

                // Retrieive the item index situated before the start time of the new item
                for ( qint32 i = start_time_ms; i >= 0; i--) {
                    if ( mPositionMsToStIndex[i] != -1) {
                        new_row_index = mPositionMsToStIndex[i] + 1;
                        break;
                    }
                }

                // Insert the new index in the lookup table
                for ( qint32 i = start_time_ms; i < mPositionMsToStIndex.size(); i++ ) {

                    if ( ( i >= start_time_ms ) && ( i <= end_time_ms) ) {
                        mPositionMsToStIndex[i] = new_row_index;
                    }
                    else if ( mPositionMsToStIndex[i] != -1 ) {
                        mPositionMsToStIndex[i] = mPositionMsToStIndex[i] + 1;
                    }
                }

                // Re-compute all subtitles index and display it
                for ( qint32 i = new_row_index; i < mStCount; i++ ) {
                    this->item(i, SUB_NUM_COL)->setText(QString::number(i + 1));
                }
                mPreviousIndex = new_row_index;
            }
            else { // Resize entry

                // If start time was modified
                if ( time_item->column() == SUB_START_TIME_COL ) {

                    // In lookup table, retrieve first occurence of the modified item
                    first_index = mPositionMsToStIndex.indexOf(row_index, 0);

                    // If this first reference is after the new start time, new subtitle duration is longer.
                    // Just need add new index reference for the new time.
                    // Else, new subtitle duration is shorter. Need to erase index reference for the old time.
                    if ( ( first_index > start_time_ms ) || ( first_index < 0 ) ) {
                        first_index = start_time_ms;
                    }
                    last_index = end_time_ms;
                } //If end time was modified
                else if ( time_item->column() == SUB_END_TIME_COL ) {

                    // In lookup table, retrieve last occurence of the modified item
                    last_index = mPositionMsToStIndex.lastIndexOf(row_index, -1);

                    // If this last reference is before the new end time, new subtitle duration is longer.
                    // Just need add new index reference for the new time.
                    // Else, new subtitle duration is shorter. Need to erase index reference for the old time.
                    if ( last_index < end_time_ms ) {
                        last_index = end_time_ms;
                    }
                    first_index = start_time_ms;
                }

                // Add or erase index of the lookup table
                for ( qint32 i = first_index; i <= last_index; i++ ) {

                    if ( ( i >= start_time_ms ) && ( i <= end_time_ms) ) {
                        mPositionMsToStIndex[i] = row_index;
                    }
                    else {
                        mPositionMsToStIndex[i] = -1;
                    }
                }
            }
        }
    }
    return true;
}

void MySubtitlesTable::updateSelectedItem() {

    qint32 start_time_ms;
    QTime start_time_HMS;
    QTime time_base(0, 0, 0, 0);

    // Center scroll area at the selected item line
    QList<QTableWidgetItem*> selected_items = this->selectedItems();
    this->scrollToItem(selected_items.first(), QAbstractItemView::PositionAtCenter);

    // If the selected item start time is valid, emit a signal sending this start time
    QString start_time_str = selected_items.at(SUB_START_TIME_COL)->text();

    start_time_HMS = QTime::fromString(start_time_str, "hh:mm:ss.zzz");

    if ( start_time_HMS.isValid() ) {
        start_time_ms = qAbs( start_time_HMS.msecsTo(time_base) );
        emit itemSelectionChanged(start_time_ms);
    }
}

void MySubtitlesTable::videoDurationChanged(qint64 videoDurationMs) {

    if ( this->isEnabled() == false) {

        this->setEnabled(true);
        mPositionMsToStIndex.resize(videoDurationMs + 1);
        mPositionMsToStIndex.fill(-1);
    }
}

void MySubtitlesTable::updateStDisplay(qint64 videoPositionMs) {

    MySubtitles subtitle;

    // Save the current player position
    mVideoPositionMs = videoPositionMs;

    qint32 st_index = mPositionMsToStIndex[videoPositionMs];

    // Check in the lookup table if there is an valid subtitle index for this position
    if ( st_index != mPreviousIndex ) {
        if ( st_index != -1 ) {
            // Retrieve the subtitle infos
            subtitle = fillSubInfos(st_index);
        }

        // Send a signal to display the subtilte or nothing if it's empty
        emit newTextToDisplay(subtitle);
    }
    mPreviousIndex = st_index;
}

MySubtitles MySubtitlesTable::fillSubInfos(qint32 stIndex) {

    MySubtitles new_subtitle;

    if ( !this->item(stIndex, SUB_START_TIME_COL)->text().isEmpty() ) {
        new_subtitle.setStartTime( this->item(stIndex, SUB_START_TIME_COL)->text() );
    }

    if ( !this->item(stIndex, SUB_END_TIME_COL)->text().isEmpty() ) {
        new_subtitle.setEndTime( this->item(stIndex, SUB_END_TIME_COL)->text() );
    }

    qint8 nbr_of_lines = this->item(stIndex, SUB_NBR_OF_LINES_COL)->text().toInt();

    if ( nbr_of_lines >= 1 ) {

        TextLine sub_line1;
        TextFont sub_font1;

        sub_font1.setFontId( this->item(stIndex, SUB_FONT_ID1_COL)->text() );
        sub_font1.setFontColor( this->item(stIndex, SUB_FONT_COLOR1_COL)->text() );
        sub_font1.setFontEffect( this->item(stIndex, SUB_FONT_EFFECT1_COL)->text() );
        sub_font1.setFontEffectColor( this->item(stIndex, SUB_FONT_EFFECTCOLOR1_COL)->text() );
        sub_font1.setFontItalic( this->item(stIndex, SUB_FONT_ITALIC1_COL)->text() );
        sub_font1.setFontUnderlined( this->item(stIndex, SUB_FONT_UNDERLINED1_COL)->text() );
        sub_font1.setFontScript( this->item(stIndex, SUB_FONT_SCRIPT1_COL)->text() );
        sub_font1.setFontSize( this->item(stIndex, SUB_FONT_SIZE1_COL)->text() );

        sub_line1.setLine( this->item(stIndex, SUB_LINE1_COL)->text() );

        sub_line1.setTextDirection( this->item(stIndex, SUB_DIRECTION1_COL)->text() );
        sub_line1.setTextHAlign( this->item(stIndex, SUB_HALIGN1_COL)->text() );
        sub_line1.setTextHPosition( this->item(stIndex, SUB_HPOSITION1_COL)->text() );
        sub_line1.setTextVAlign( this->item(stIndex, SUB_VALIGN1_COL)->text() );
        sub_line1.setTextVPosition( this->item(stIndex, SUB_VPOSITION1_COL)->text() );

        new_subtitle.setText(sub_line1, sub_font1);
    }

    if ( nbr_of_lines >= 2 ) {

        TextLine sub_line2;
        TextFont sub_font2;

        sub_font2.setFontId( this->item(stIndex, SUB_FONT_ID2_COL)->text() );
        sub_font2.setFontColor( this->item(stIndex, SUB_FONT_COLOR2_COL)->text() );
        sub_font2.setFontEffect( this->item(stIndex, SUB_FONT_EFFECT2_COL)->text() );
        sub_font2.setFontEffectColor( this->item(stIndex, SUB_FONT_EFFECTCOLOR2_COL)->text() );
        sub_font2.setFontItalic( this->item(stIndex, SUB_FONT_ITALIC2_COL)->text() );
        sub_font2.setFontUnderlined( this->item(stIndex, SUB_FONT_UNDERLINED2_COL)->text() );
        sub_font2.setFontScript( this->item(stIndex, SUB_FONT_SCRIPT2_COL)->text() );
        sub_font2.setFontSize( this->item(stIndex, SUB_FONT_SIZE2_COL)->text() );

        sub_line2.setLine( this->item(stIndex, SUB_LINE2_COL)->text() );

        sub_line2.setTextDirection( this->item(stIndex, SUB_DIRECTION2_COL)->text() );
        sub_line2.setTextHAlign( this->item(stIndex, SUB_HALIGN2_COL)->text() );
        sub_line2.setTextHPosition( this->item(stIndex, SUB_HPOSITION2_COL)->text() );
        sub_line2.setTextVAlign( this->item(stIndex, SUB_VALIGN2_COL)->text() );
        sub_line2.setTextVPosition( this->item(stIndex, SUB_VPOSITION2_COL)->text() );

        new_subtitle.setText(sub_line2, sub_font2);
    }

    return new_subtitle;
}
