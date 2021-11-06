#include "proxystyle.h"
#include <QRadialGradient>
#include <QDebug>
#include "commoninclude.h"

#define ARR_DRAW_POLYGON
//#define PUSHBUTTON_TEXT_BOLD

QSize TabBarStyle::sizeFromContents(QStyle::ContentsType type, const QStyleOption *option, const QSize &size, const QWidget *widget) const
{
    QSize t = QProxyStyle::sizeFromContents(type, option, size, widget);
    if (type == QStyle::CT_TabBarTab) {
        t.transpose();
        t.rwidth() = TABBAR_LABEL_WIDTH;
        t.rheight() = TABBAR_LABEL_HEIGHT;
    }
    return t;
}

void TabBarStyle::drawControl(QStyle::ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    Q_UNUSED(widget)
    if (element == CE_TabBarTabLabel) {
        if (const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(option)) {
            QRect labelRect = tab->rect;
            // 抗锯齿
            painter->setRenderHint(QPainter::Antialiasing);
            if (tab->state & State_Selected){
                painter->save();
                // 选中的tab bar 的边框颜色
                painter->setPen(QColor(55,57,55));
                // 选中的tab bar 的填充色
                painter->setBrush(QBrush(QColor(150,153,153)));
                // 选中的tab bar 的填充形状 （y+2）
                QPoint ps[5]= {labelRect.topLeft(),labelRect.topRight(),
                                QPoint((labelRect.left() + 4*labelRect.right())*0.2,(labelRect.top() + labelRect.bottom())*0.5+2),
                                labelRect.bottomRight(), labelRect.bottomLeft()};
                painter->drawPolygon(ps,5);
                painter->restore();
                painter->save();
                painter->setPen(QColor(71,71,71));
                painter->setBrush(QBrush(QColor(71,71,71)));
                QPoint shadowPoints[8] = {labelRect.topLeft(),labelRect.topRight(),
                                          QPoint(labelRect.topRight().x()    - TABBAR_SHADOW_WIDTH, labelRect.topRight().y()    + TABBAR_SHADOW_WIDTH),
                                          QPoint(labelRect.topLeft().x()     + TABBAR_SHADOW_WIDTH, labelRect.topLeft().y()     + TABBAR_SHADOW_WIDTH),
                                          QPoint(labelRect.bottomLeft().x()  + TABBAR_SHADOW_WIDTH, labelRect.bottomLeft().y()  - TABBAR_SHADOW_WIDTH),
                                          QPoint(labelRect.bottomRight().x() - TABBAR_SHADOW_WIDTH, labelRect.bottomRight().y() - TABBAR_SHADOW_WIDTH),
                                          labelRect.bottomRight(),
                                          labelRect.bottomLeft()};
                painter->drawPolygon(shadowPoints, 8);
                painter->restore();
            }
            else if(tab->state & State_MouseOver){
                // mouseover 在border会有颜色残留bug
                // 所以 setPen设置边缘为背景颜色
                painter->save();
                painter->setPen(QColor(TABBAR_BACKGROUND_COLOR));
                painter->setBrush(QColor(56,56,53));
                painter->drawRect(labelRect);
                painter->restore();
            }
            else{
                // 平时背景设为与背景颜色一致，透明pushbutton
                painter->save();
                painter->setPen(QColor(TABBAR_BACKGROUND_COLOR));
                painter->setBrush(QColor(TABBAR_BACKGROUND_COLOR));
                painter->drawRect(labelRect);
                painter->restore();
            }
            painter->save();
            QTextOption optionS;
            // 文字居中，默认文字方向就是水平的
            optionS.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
            // selected 与 !selected的tab bar 的文字颜色
            (tab->state & QStyle::State_Selected) ? painter->setPen(QColor(40,41,42)) : painter->setPen(QColor(225,225,225));
            painter->drawText(labelRect.adjusted(int(labelRect.width()*0.382),0,0,0), tab->text, optionS);
            painter->restore();

            // 设置图标
            const int iconWidth = 30;
            const int iconHeight = 30;
            QRect iconRect(labelRect.topLeft().x() + int(labelRect.width()*0.382-iconWidth)-5
                           , labelRect.topLeft().y() + int(labelRect.height()*0.5-iconHeight*0.5)
                               , iconWidth,iconHeight);
            painter->save();
            if(tab->state & QStyle::State_Selected){
                painter->drawPixmap(iconRect, tab->icon.pixmap(QSize(iconWidth,iconHeight), QIcon::Mode::Selected));
            }
            else{
                painter->drawPixmap(iconRect, tab->icon.pixmap(QSize(iconWidth,iconHeight), QIcon::Mode::Normal));
            }
            painter->restore();
            return;
        }
    }
    if(element == CE_TabBarTab){
        QProxyStyle::drawControl(element,option,painter,widget);
    }
}


void TabWidgetStyle::drawPrimitive(QStyle::PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    if(element == PE_FrameTabWidget){
        if(const QStyleOptionTabWidgetFrame *tableWF = qstyleoption_cast<const QStyleOptionTabWidgetFrame *>(option)){
            QRect r=widget->rect();
            // TabWidget右侧border缺的6px涂色
            painter->setRenderHint(QPainter::Antialiasing);
            painter->save();
            painter->setPen(QColor(61,61,61));
            QPoint po[2] = {r.topRight(), r.bottomRight()};
            painter->drawPolyline(po,2);
            painter->restore();
            // QTabWidget背景，最上面那一排放按钮的地方的背景不要涂
            painter->save();
            // 从左到右的渐变
            //            QLinearGradient g(QPoint(r.topLeft().x()+tabBarLabelWidth-1, r.topLeft().y()+button_up_height+TabWidget_menu_horizontal_margin),
            //                              QPoint(r.topRight().x(), r.topRight().y()+button_up_height+TabWidget_menu_horizontal_margin));
            //            g.setColorAt(0,QColor(51,51,51));
            //            g.setColorAt(1,QColor(61,61,61));
            painter->setPen(QColor(51,51,51));
            painter->setBrush(QColor(51,51,51));
            painter->drawRect(r.adjusted(TABBAR_LABEL_WIDTH, TABWIDGET_BUTTON_UP_HEIGHT+TABWIDGET_MENU_HORIZONTAL_MARGIN,0,0));
            painter->restore();
            // QTabWidget最上面那一排放按钮的地方的背景
            painter->save();
            painter->setPen(QColor(TABWIDGET_MENU_BACKGROUND_COLOR));
            painter->setBrush(QColor(TABWIDGET_MENU_BACKGROUND_COLOR));
            painter->drawRect(r.adjusted(TABBAR_LABEL_WIDTH,0,0, TABWIDGET_BUTTON_UP_HEIGHT+TABWIDGET_MENU_HORIZONTAL_MARGIN-r.height()));
            painter->restore();
            return;
        }
    }
    QProxyStyle::drawPrimitive(element, option, painter, widget);

}

QRect TabWidgetStyle::subElementRect(QStyle::SubElement element, const QStyleOption *option, const QWidget *widget) const
{
    QRect rect  = QProxyStyle::subElementRect(element, option, widget);
    if(element == SE_TabWidgetTabBar){
        // 把tabbar向下移一段距离，这段距离的长度是顶端要显示的那一行按钮的高度 + offset_width
        rect.adjust(0,TABWIDGET_BUTTON_UP_HEIGHT+TABWIDGET_MENU_HORIZONTAL_MARGIN,0, TABWIDGET_BUTTON_UP_HEIGHT+TABWIDGET_MENU_HORIZONTAL_MARGIN);
    }
    return rect;
}

void TabWidgetStyle::drawControl(QStyle::ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    QProxyStyle::drawControl(element, option, painter, widget);
}

const int icon_offset = 7;                  // 图标左侧和border的距离
const int text_offset = 3;                  // 文字左侧和图标的距离
const int pushButton_iconWidth = 30;        // 图标的长度和宽度
const int pushButton_iconHeight = 30;

void PushButtonStyle::drawControl(QStyle::ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    Q_UNUSED(widget)
    if(element == CE_PushButton){
        if(const QStyleOptionButton *pb = qstyleoption_cast<const QStyleOptionButton *>(option)){
            QRect labelRect = pb->rect;
            // 抗锯齿
            painter->setRenderHint(QPainter::Antialiasing);
            if(pb->state & State_Enabled){
                // pressed
                if(pb->state & State_Sunken){
                    // 背景颜色
                    painter->save();
                    painter->setPen(QPen(QColor(55,85,100), 2));
                    painter->setBrush(QColor(51,51,51));
                    painter->drawRect(labelRect);
                    painter->restore();
                }
                // hover
                else if(pb->state & State_MouseOver){
                    // 背景颜色
                    painter->save();
                    painter->setPen(QPen(QColor(55,85,100), 2));
                    painter->setBrush(QColor(64,64,61));
                    painter->drawRect(labelRect);
                    painter->restore();
                }
                // 一般情况
                else{
                    // 背景颜色
                    painter->save();
                    painter->setPen(Qt::transparent);
                    painter->setBrush(Qt::transparent);
                    // 不要用painter->setCompositionMode(QPainter::CompositionMode_Clear);，会把border的setPen覆盖掉使之失效
                    //                    painter->setCompositionMode(QPainter::CompositionMode_Clear);
                    //                    painter->setBackgroundMode(Qt::TransparentMode);
                    painter->drawRect(labelRect);
                    painter->restore();
                }
                // 文字颜色
                painter->save();
#ifdef PUSHBUTTON_TEXT_BOLD
                QFont f = painter->font();
                f.setBold(true);
                painter->setFont(f);
#endif
                QTextOption optionS;
                optionS.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
                painter->setPen(QColor(240,240,240));
                painter->drawText(labelRect.adjusted(icon_offset+pushButton_iconWidth+text_offset,0,0,0), pb->text, optionS);
                painter->restore();
            }
            // diabled
            else{
                painter->save();
                painter->setPen(QColor(PUSHBUTTON_DISABLE_COLOR));
                painter->setBrush(QColor(PUSHBUTTON_DISABLE_COLOR));
                painter->drawRect(labelRect);
                painter->restore();
                // disabled 文字颜色
                painter->save();
#ifdef PUSHBUTTON_TEXT_BOLD
                QFont f = painter->font();
                f.setBold(true);
                painter->setFont(f);
#endif
                QTextOption optionS;
                optionS.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
                painter->setPen(Qt::gray);
                painter->drawText(labelRect.adjusted(icon_offset+pushButton_iconWidth+text_offset,0,0,0), pb->text, optionS);
                painter->restore();


            }

            // 设置图标
            painter->save();
            QRect iconRect(labelRect.topLeft().x()+icon_offset
                           , labelRect.topLeft().y()+TABWIDGET_MENU_VERTICAL_MARGIN
                           , pushButton_iconWidth,pushButton_iconHeight);
            painter->drawPixmap(iconRect, pb->icon.pixmap(QSize(pushButton_iconWidth, pushButton_iconHeight), QIcon::Mode::Normal));
            painter->restore();

            return;
        }
    }

    QProxyStyle::drawControl(element, option, painter, widget);

}

void HorizontalScrollBarStyle::drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const
{
    if(control == CC_ScrollBar){
        if(const QStyleOptionSlider *sli = qstyleoption_cast<const QStyleOptionSlider *>(option)){
            QRect sliRect = subControlRect(CC_ScrollBar, option, SC_ScrollBarSlider, widget);
            QRect grooveRect = subControlRect(CC_ScrollBar, option, SC_ScrollBarGroove, widget);
            QRect upRect = subControlRect(CC_ScrollBar, option, SC_ScrollBarSubLine, widget);
            QRect downRect = subControlRect(CC_ScrollBar, option, SC_ScrollBarAddLine, widget);
            // 抗锯齿
            painter->setRenderHint(QPainter::Antialiasing);
            painter->save();
            painter->setPen(QColor(201,204,204));
            painter->setBrush(QColor(201,204,204));
            QPoint addGround[4] = {grooveRect.topLeft(), grooveRect.topRight(), grooveRect.bottomRight(), grooveRect.bottomLeft()};
            painter->drawPolygon(addGround,4);
            painter->restore();
            // ScrollBar的slider(handle)
            if(sli->state & QStyle::State_Sunken){
                painter->save();
                painter->setPen(QColor(51,51,51));
                QLinearGradient g(sliRect.topRight(), sliRect.bottomRight());
                g.setColorAt(0,QColor(51,51,51));
                g.setColorAt(1,QColor(61,61,61));
                painter->setBrush(g);
                QPoint backGround[4] = {sliRect.topLeft(), sliRect.topRight(), sliRect.bottomRight(), sliRect.bottomLeft()};
                painter->drawPolygon(backGround,4);
                painter->restore();
            }
            else if(sli->state & QStyle::State_MouseOver){
                painter->save();
                painter->setPen(QColor(85,85,90));
                QLinearGradient g(sliRect.topRight(), sliRect.bottomRight());
                g.setColorAt(0,QColor(85,85,90));
                g.setColorAt(1,QColor(95,100,100));
                painter->setBrush(g);
                QPoint backGround[4] = {sliRect.topLeft(), sliRect.topRight(), sliRect.bottomRight(), sliRect.bottomLeft()};
                painter->drawPolygon(backGround,4);
                painter->restore();
            }
            else{
                painter->save();
                painter->setPen(QColor(120,125,130));
                QLinearGradient g(sliRect.topRight(), sliRect.bottomRight());
                g.setColorAt(0,QColor(120,125,130));
                g.setColorAt(1,QColor(132,135,140));
                painter->setBrush(g);
                QPoint backGround[4] = {sliRect.topLeft(), sliRect.topRight(), sliRect.bottomRight(), sliRect.bottomLeft()};
                painter->drawPolygon(backGround,4);
                painter->restore();
            }
            // 两个arrow所在的方框
            painter->save();
            painter->setPen(QColor(51,51,51));
            painter->setBrush(QColor(240,255,255));
            QPoint up[4] = {upRect.topLeft(), upRect.topRight(), upRect.bottomRight(), upRect.bottomLeft()};
            QPoint down[4] = {downRect.topLeft(), downRect.topRight(), downRect.bottomRight(), downRect.bottomLeft()};
            painter->drawPolygon(up,4);
            painter->drawPolygon(down,4);
            painter->restore();

                // arrow的颜色和形状
                painter->save();
            painter->setPen(QColor(240,255,255));
            painter->setBrush(QColor(51,51,51));
            int topRightY = upRect.bottomLeft().y();
            int topwidth = upRect.width();
            int downRightX = downRect.x();
            QPoint leftArr[3] = {
                QPoint(PADDING_UP, topRightY*0.5),
                QPoint(topwidth-PADDING_DOWN, PADDING_LEFT),
                QPoint(topwidth-PADDING_DOWN, topRightY-PADDING_LEFT),
                };
            QPoint rightArr[3] = {
                QPoint(PADDING_DOWN+downRightX, PADDING_LEFT),
                QPoint(PADDING_DOWN+downRightX, topRightY-PADDING_LEFT),
                QPoint(topwidth-PADDING_DOWN+downRightX, topwidth*0.5),
                };
            painter->drawPolygon(leftArr,3);
            painter->drawPolygon(rightArr,3);
            painter->restore();
            return;
        }
    }
    else{
        drawComplexControl(control, option, painter, widget);
    }
}



void VerticalScrollBarStyle::drawComplexControl(QStyle::ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const
{
    if(control == CC_ScrollBar){
        if(const QStyleOptionSlider *sli = qstyleoption_cast<const QStyleOptionSlider *>(option)){
            QRect sliRect = subControlRect(CC_ScrollBar, option, SC_ScrollBarSlider, widget);
            QRect grooveRect = subControlRect(CC_ScrollBar, option, SC_ScrollBarGroove, widget);
            // upRect上箭头的方框
            QRect upRect = subControlRect(CC_ScrollBar, option, SC_ScrollBarSubLine, widget);
            // downRect下箭头的方框
            QRect downRect = subControlRect(CC_ScrollBar, option, SC_ScrollBarAddLine, widget);
            // 抗锯齿
            painter->setRenderHint(QPainter::Antialiasing);
            painter->save();
            painter->setPen(QColor(201,204,204));
            painter->setBrush(QColor(201,204,204));
            QPoint addGround[4] = {grooveRect.topLeft(), grooveRect.topRight(), grooveRect.bottomRight(), grooveRect.bottomLeft()};
            painter->drawPolygon(addGround,4);
            painter->restore();
            // ScrollBar的slider(handle)
            if(sli->state & QStyle::State_Sunken){
                painter->save();
                painter->setPen(QColor(51,51,51));
                QLinearGradient g(sliRect.topRight(), sliRect.bottomRight());
                g.setColorAt(0,QColor(51,51,51));
                g.setColorAt(1,QColor(61,61,61));
                painter->setBrush(g);
                QPoint backGround[4] = {sliRect.topLeft(), sliRect.topRight(), sliRect.bottomRight(), sliRect.bottomLeft()};
                painter->drawPolygon(backGround,4);
                painter->restore();
            }
            else if(sli->state & QStyle::State_MouseOver){
                painter->save();
                painter->setPen(QColor(85,85,90));
                QLinearGradient g(sliRect.topRight(), sliRect.bottomRight());
                g.setColorAt(0,QColor(85,85,90));
                g.setColorAt(1,QColor(95,100,100));
                painter->setBrush(g);
                QPoint backGround[4] = {sliRect.topLeft(), sliRect.topRight(), sliRect.bottomRight(), sliRect.bottomLeft()};
                painter->drawPolygon(backGround,4);
                painter->restore();
            }
            else{
                painter->save();
                painter->setPen(QColor(120,125,130));
                QLinearGradient g(sliRect.topRight(), sliRect.bottomRight());
                g.setColorAt(0,QColor(120,125,130));
                g.setColorAt(1,QColor(132,135,140));
                painter->setBrush(g);
                QPoint backGround[4] = {sliRect.topLeft(), sliRect.topRight(), sliRect.bottomRight(), sliRect.bottomLeft()};
                painter->drawPolygon(backGround,4);
                painter->restore();
            }
            // 两个arrow所在的方框
            painter->save();
            painter->setPen(QColor(51,51,51));
            painter->setBrush(QColor(240,255,255));
            QPoint up[4] = {upRect.topLeft(), upRect.topRight(), upRect.bottomRight(), upRect.bottomLeft()};
            QPoint down[4] = {downRect.topLeft(), downRect.topRight(), downRect.bottomRight(), downRect.bottomLeft()};
            painter->drawPolygon(up,4);
            painter->drawPolygon(down,4);
            painter->restore();

                // arrow的颜色和形状
                painter->save();
            painter->setPen(QColor(240,255,255));
            painter->setBrush(QColor(51,51,51));
            // VerticalScrollBar，坐标轴原点在所在方框的左上角，x轴水平向右，y轴竖直向下
            int topRightX = upRect.topRight().x();
            int topHeight = upRect.height();
            // downRightY控制把下箭头移动到下箭头该在的方框里
            int downRightY = downRect.y();
            QPoint upArr[3] = {
                QPoint(topRightX*0.5, PADDING_UP),
                QPoint(PADDING_LEFT, topHeight-PADDING_DOWN),
                QPoint(topRightX-PADDING_LEFT, topHeight-PADDING_DOWN),
                };
            QPoint downArr[3] = {
                // y必须 + downRightY，才能正确绘制到下箭头所在的方框里
                QPoint(PADDING_LEFT, PADDING_DOWN+downRightY),
                QPoint(topRightX-PADDING_LEFT, PADDING_DOWN+downRightY),
                QPoint(topHeight*0.5, topHeight-PADDING_DOWN+downRightY),
                };
            painter->drawPolygon(upArr,3);
            painter->drawPolygon(downArr,3);
            painter->restore();
            return;
        }
    }
    else{
        drawComplexControl(control, option, painter, widget);
    }

}

void CheckBoxStyle::drawPrimitive(QStyle::PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    Q_UNUSED(widget);
    if(element == PE_IndicatorCheckBox){
        if(const QStyleOptionButton *cb = qstyleoption_cast<const QStyleOptionButton *>(option)){
            // iRect方向：左上角为顶点，x轴水平向右，y轴竖直向下
            QRect iRect= cb->rect;
            painter->setRenderHint(QPainter::Antialiasing);
            // disabled 绘制打勾方框
            if(!(cb->state & State_Enabled)){
                painter->save();
                painter->setPen(QColor(CHECKBOX_DIABLE_COLOR));
                painter->setBrush(QBrush(QColor(CHECKBOX_DIABLE_COLOR)));
                painter->drawRect(iRect);
                painter->restore();
            }
            else if(cb->state & State_On){
#ifdef CHECKBOX_USE_RG
                // 方框内使用圆形渐变，而不是打对勾
                QRadialGradient indicatorColorGradient(QPointF(iRect.topLeft().x()*0.5 + iRect.bottomRight().x()*0.5, iRect.topLeft().y()*0.5 + iRect.bottomRight().y()*0.5),
                                                       15,
                                                       QPointF(iRect.topLeft().x()*0.5 + iRect.bottomRight().x()*0.5, iRect.topLeft().y()*0.5 + iRect.bottomRight().y()*0.5));
                indicatorColorGradient.setColorAt(0, QColor(40,65,90));
                indicatorColorGradient.setColorAt(0.9, QColor(115,120,140));
                indicatorColorGradient.setColorAt(1, QColor(240,255,255));
#else
                // 方框内打对勾，而不是使用圆形渐变
                // 将整个方框的长和宽分别六等分，对勾的下边缘线条所在位置为(1,3) -> (2,5) -> (5,1)，按设定的宽度向上拓宽一个宽度（x轴水平向右，y轴竖直向下）
                //                QRect checkRect = QRect(iRect.topLeft().x()*0.5 + iRect.bottomRight().x()*0.5, iRect.topLeft().y()*0.5 + iRect.bottomRight().y()*0.5, 15, 15);
                // NOTE: 我也不知道为什么要用这个比例，根据QRect(0,0 16x16)
                qreal w6 = iRect.width()/5;
                qreal h6 = iRect.height()/5;
                //                qreal downLeftX = iRect.bottomLeft().x()*0.5;
                qreal downLeftY = iRect.topLeft().y();

                // 用drowPolygon沿着下边缘画
                //                QPointF check[6] = {
                //                    // 下边缘从左到右三个点
                //                    QPointF(downLeftX+w6, downLeftY+h6*4), QPointF(downLeftX+w6*3, downLeftY+h6*6), QPointF(downLeftX+w6*6, downLeftY+h6*3),
                //                    // 上边缘从左到右三个点，多向上一个CHECKBOX_CHECK_WIDTH
                //                    QPointF(downLeftX+w6*6, downLeftY+h6*3-CHECKBOX_CHECK_WIDTH),  QPointF(downLeftX+w6*3, downLeftY+h6*6-CHECKBOX_CHECK_WIDTH), QPointF(downLeftX+w6, downLeftY+h6*4-CHECKBOX_CHECK_WIDTH)
                //                };
                QPointF check[6] = {
                    // 下边缘从左到右三个点
                    QPointF(0, downLeftY+h6*4), QPointF(w6*2, downLeftY+h6*5), QPointF(w6*5, downLeftY+h6*3),
                    // 上边缘从右到左三个点，多向上一个CHECKBOX_CHECK_WIDTH
                    QPointF(w6*5, downLeftY+h6),  QPointF(w6*2, downLeftY+h6*3), QPointF(0, downLeftY+h6*2)
                };
#endif

                painter->save();
#ifdef CHECKBOX_USE_RG
                painter->setBrush(QBrush(indicatorColorGradient));
#else
                painter->setPen(QColor(240,255,255));
                painter->setBrush(QBrush(QColor(240,255,255)));
                painter->drawRect(iRect);
                painter->setPen(QColor(115,120,140));
                painter->setBrush(QColor(55,85,100));
                painter->drawPolygon(check, 6);
#endif
                //                painter->drawRect(iRect);
                painter->restore();
            }
            else{
                painter->save();
                painter->setPen(QColor(240,255,255));
                painter->setBrush(QBrush(QColor(240,255,255)));
                painter->drawRect(iRect);
                painter->restore();
            }
        }
    }
    return;
}

void RadioButtonStyle::drawPrimitive(QStyle::PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    Q_UNUSED(widget);
    if(element == PE_IndicatorRadioButton){
        if(const QStyleOptionButton *rb = qstyleoption_cast<const QStyleOptionButton *>(option)){
            QRect rRect = rb->rect;
            painter->setRenderHint(QPainter::Antialiasing);
            painter->save();
            painter->setPen(QColor(115,120,140));
            painter->setBrush(QBrush(QColor(240,255,255)));
            painter->drawRoundedRect(rRect, rRect.width()*0.5, rRect.height()*0.5);
            painter->restore();
            // QCheckBox 选中后的点
            if(rb->state & State_On){
                painter->save();
                painter->setPen(QColor(115,120,140));
                painter->setBrush(QColor(55,85,100));
                painter->drawEllipse(rRect.adjusted(rRect.width()*0.25, rRect.height()*0.25, -rRect.width()*0.25, -rRect.height()*0.25));
                painter->restore();
            }
        }
    }
    return;
}

void ComboBoxStyle::drawControl(QStyle::ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    if(element == CE_ComboBoxLabel){
        if(const QStyleOptionComboBox *cb = qstyleoption_cast<const QStyleOptionComboBox *>(option)){
            QRect labelRect = cb->rect;
            painter->setRenderHint(QPainter::Antialiasing);

            painter->save();
            painter->setBrush(QColor(Qt::transparent));
            painter->setPen(QPen(QColor(55,85,100), 2));
            painter->drawRect(labelRect);
            painter->restore();

            painter->save();
            QTextOption optionS;
            optionS.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
            painter->setPen(QPen(QColor(240,255,255)));
            painter->drawText(labelRect.adjusted(4,0,0,0), cb->currentText, optionS);
            painter->restore();

            return;
        }
    }
    else if(element == CE_ItemViewItem){
        if(const QStyleOptionViewItem *vi = qstyleoption_cast<const QStyleOptionViewItem *>(option)){
            QRect itemRect = vi->rect;
            painter->setRenderHint(QPainter::Antialiasing);
            painter->save();
            painter->setPen(Qt::transparent);
            if(vi->state & State_MouseOver){
                painter->setBrush(QColor(55,85,100));
            }
            else{
                // comboBox设置背景为透明时，如果其下拉列表的item的背景也要设置成透明，
                // 则需要从comboBox的parent的pallete处获取background的颜色。
                // 千里追魂：ViewItem -> QComboBoxPrivateContainer -> ComboBox -> ComboBox的parent
                // 设置下拉列表背景为透明：
                // （1）若comboBox位置的背景颜色为固定颜色，直接获取该颜色并设置即可
                // （2）若comboBox位置的背景颜色为渐变颜色，只能取该渐变的中间色近似模拟
                // （3）若comboBox位置的背景颜色既不固定也不是渐变，那么？？
                //
                // FIXME：
                // 目前只考虑comboBox的parent背景有颜色的情况，如果parent也没颜色（透明）
                // 是否需要再往上找？
                const QBrush parentPainter = widget->parentWidget()->parentWidget()->parentWidget()->palette().window();
                if(parentPainter.style() == Qt::LinearGradientPattern
                    || parentPainter.style() == Qt::ConicalGradientPattern
                    || parentPainter.style() == Qt::RadialGradientPattern){
                    QGradientStops stops = parentPainter.gradient()->stops();
                    const QColor startColor = stops.first().second;
                    const QColor stopColor  = stops.last().second;
                    painter->setBrush(QColor((startColor.red()   + stopColor.red())*0.5,
                                             (startColor.green() + stopColor.green())*0.5,
                                             (startColor.blue()  + stopColor.blue())*0.5));
                }
                else{
                    painter->setBrush(parentPainter.color());
                }
            }
            painter->drawRect(itemRect);
            painter->restore();

            painter->save();
            QTextOption optionS;
            optionS.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
            painter->setPen(QPen(QColor(240,255,255)));
            painter->drawText(itemRect.adjusted(4,0,0,0), vi->text, optionS);
            painter->restore();

//            QRect c = widget->parentWidget()->rect();
////            painter->setPen(QPen(QColor(255,0,0), 5));
//            painter->setPen(QColor(55,85,100));
//            painter->drawRect(c);


            return;
        }
    }
    else{
        QProxyStyle::drawControl(element, option, painter, widget);
    }
}


void ComboBoxStyle::drawComplexControl(QStyle::ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const
{
    if(control == CC_ComboBox){
        if(const QStyleOptionComboBox *cb = qstyleoption_cast<const QStyleOptionComboBox *>(option)){
//            QRect focusRect = subControlRect(CC_ComboBox, option, SC_ComboBoxFrame, widget);
//            QRect editRect = subControlRect(CC_ComboBox, option, SC_ComboBoxEditField, widget);
//            QRect listPopupRect = subControlRect(CC_ComboBox, option, SC_ComboBoxListBoxPopup, widget);
            QRect arrowRect = subControlRect(CC_ComboBox, option, SC_ComboBoxArrow, widget);
            painter->setRenderHint(QPainter::Antialiasing);

//            painter->save();
//            painter->setBackgroundMode(Qt::TransparentMode);
//            painter->setPen(QPen(QColor(255,0,0), 4));
//            painter->drawRect(focusRect);
//            painter->restore();

//            painter->save();
//            painter->setBackgroundMode(Qt::TransparentMode);
//            painter->drawRect(editRect);
//            painter->restore();

//            painter->save();
//            painter->setBackgroundMode(Qt::TransparentMode);
//            painter->setPen(QPen(QColor(255,0,0), 4));
//            painter->drawRect(listPopupRect);
//            painter->restore();
//            qDebug() << listPopupRect;

            painter->save();
            painter->setPen(QPen(QColor(240,255,255), 2));
            int topHeight = arrowRect.height();
            QPoint downArr[3] = {
                QPoint(arrowRect.x() + PADDING_LEFT                    , topHeight*0.5 - 2),
                QPoint(arrowRect.x() + arrowRect.width() * 0.5         , topHeight*0.5 + 2),
                QPoint(arrowRect.x() + arrowRect.width() - PADDING_LEFT, topHeight*0.5 - 2)
            };
            painter->drawPolyline(downArr,3);
            painter->restore();

//            painter->save();
//            QRect popupRect = cb->popupRect;
//            qDebug() << popupRect;
//            painter->setPen(QPen(QColor(255,0,0),4));
//            painter->setBrush(QColor(0,255,0));
//            painter->drawRect(popupRect);
//            painter->restore();
            return;
        }
    }
    QProxyStyle::drawComplexControl(control, option, painter, widget);
}

//void ComboBoxStyle::drawPrimitive(QStyle::PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
//{

//}

void TreeViewStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    if(element == PE_IndicatorBranch){
        if(const QStyleOption *tvb = qstyleoption_cast<const QStyleOption *>(option)){
            // 展开children用的branch
            if(tvb->state & State_Children){
                QRect branchRect = tvb->rect;
                painter->setRenderHint(QPainter::Antialiasing);
                painter->save();
                painter->setPen(QPen(tvb->state & State_MouseOver ? QColor(57,95,108) : QColor(240,255,255), 2));
                painter->setBrush(tvb->state & State_MouseOver ? QColor(240,255,255) : QColor(57,95,108));
                // for branchRect size = 20x21(default)
                int basePosX = branchRect.x() + branchRect.width()*0.5;
                int basePosY = branchRect.y() + 0.5*branchRect.height();
                QPoint branchPos[3];
                // branch expanded
                if(tvb->state & State_Open){
                    branchPos[0] = QPoint(basePosX - 6, basePosY - 4);
                    branchPos[1] = QPoint(basePosX    , basePosY + 4);
                    branchPos[2] = QPoint(basePosX + 6, basePosY - 4);
                }
                else{
                    branchPos[0] = QPoint(basePosX - 4, basePosY - 6);
                    branchPos[1] = QPoint(basePosX + 4, basePosY    );
                    branchPos[2] = QPoint(basePosX - 4, basePosY + 6);
                }
                painter->drawPolygon(branchPos, 3);
                painter->restore();
                return;
            }
        }
    }
    // use PE_PanelItemViewItem instead of PE_PanelItemViewRow if want to paint when mouseover or selected
    else if(element == PE_PanelItemViewItem){
        if(const QStyleOptionViewItem  *tvi = qstyleoption_cast<const QStyleOptionViewItem *>(option)){
            QRect rowRect = tvi->rect;
            painter->setRenderHint(QPainter::Antialiasing);
            painter->save();
            painter->setPen(Qt::transparent);
            if(tvi->state & State_MouseOver){
                painter->setBrush(QColor(37,65,88));
            }
            else if(tvi->state & State_Selected){
                painter->setBrush(QColor(30,59,82));
            }
            else{
                painter->setBrush(Qt::transparent);
            }
            painter->drawRect(rowRect);
            painter->restore();
        }
    }
    return;
    // do not use this
    QProxyStyle::drawPrimitive(element, option, painter, widget);
}
