#ifndef TEXTBOX_H
#define TEXTBOX_H

void DrawTextBoxed(Font font, const char *text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint); //텍스트 박스 그리기
void DrawTextBoxedSelectable(Font font, const char *text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint, int selectStart, int selectLength, Color selectTint, Color selectBackTint); // 텍스트 박스 선택됨 그리기


#endif