/**
 * @file buttonwidget.h
 * @author Riccardo Iacob
 * @brief Button widget abstraction class for TFT_eSPI library
 * @version 0.1
 * @date 2023-07-10
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef BUTTONWIDGET_H
#define BUTTONWIDGET_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "debug.h"

class ButtonWidget
{
public:
    /**
     * @brief Possible button styles
     *
     */
    enum class ButtonStyles
    {
        RECT,
        ROUND_RECT,
        ELLIPSE
    };

    enum class TooltipPositions
    {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

private:
    // X touch position
    uint16_t *_touchx;
    // Y touch position
    uint16_t *_touchy;
    // Pointer to the tft display
    TFT_eSPI *_tft;
    // X starting coordinate of the button
    uint16_t _startx;
    // Y starting coordinate of the button
    uint16_t _starty;
    // X size of the button
    uint16_t _sizex;
    // Y size of the button
    uint16_t _sizey;
    // Button icon
    uint8_t *_icon = {0};
    // Button text
    char *_text = "";
    // Button style
    ButtonStyles _style = ButtonStyles::RECT;
    // Background color
    uint16_t _bgcolor = TFT_WHITE;
    // Foreground color
    uint16_t _fgcolor = TFT_BLACK;
    // Radius of the corner smoothing (if ButtonStyle is ROUND_RECT)
    uint16_t _cornerradius = 5;
    // _hasIcon = true if the user calls setIcon
    // _hasIcon = false if the user calls setText
    bool _hasIcon = false;
    // Font size
    uint8_t _fontSize = 1;
    // Font
    uint8_t _font = 2;

    bool _tooltipPresent = false;
    char *_tooltip = "";
    uint8_t _tooltipFont = 2;
    uint8_t _tooltipFontSize = 1;
    TooltipPositions _tooltipPosition = TooltipPositions::RIGHT;
    uint16_t _tooltipPadding = 10;
    uint16_t _tooltipFgColor = TFT_BLACK;

public:
    /**
     * @brief Constructs a new Touch Button object
     *
     * @param touchx: Pointer to the current touch X position
     * @param touchy: Pointer to the current touch Y position
     * @param tft: Pointer to the TFT screen object
     * @param startx: Start X cordinate of the button (and its bounding box)
     * @param starty: Start Y cordinate of the button (and its bounding box)
     * @param sizex: X size of the button
     * @param sizey: Y size of the button
     */
    ButtonWidget(uint16_t *touchx, uint16_t *touchy, TFT_eSPI *tft, uint16_t startx, uint16_t starty, uint16_t sizex, uint16_t sizey)
    {
        _touchx = touchx;
        _touchy = touchy;
        _tft = tft;
        _startx = startx;
        _starty = starty;
        _sizex = sizex;
        _sizey = sizey;
    }

    /**
     * @brief Sets the style of the button
     *
     * @param bgcolor: Background color
     * @param fgcolor: Foreground (text or icon) color
     * @param style: Type of container
     * @param cornerradius: Optional and only needed if using ROUND_RECT, corner smoothing radius
     */
    void setStyle(uint16_t bgcolor, uint16_t fgcolor, ButtonStyles style, uint16_t cornerradius = 5)
    {
        _bgcolor = bgcolor;
        _fgcolor = fgcolor;
        _style = style;
        _cornerradius = cornerradius;
    }

    /**
     * @brief Draws a button according to the set style parameters
     *
     */
    void draw()
    {
        // Draw background
        switch (_style)
        {
        case ButtonStyles::ROUND_RECT:
        {
            _tft->fillRoundRect(_startx, _starty, _sizex, _sizey, _cornerradius, _bgcolor);
            break;
        }
        case ButtonStyles::RECT:
        {
            _tft->fillRect(_startx, _starty, _sizex, _sizey, _bgcolor);
            break;
        }
        case ButtonStyles::ELLIPSE:
        {
            _tft->fillEllipse(_startx + (_sizex / 2), _starty + (_sizey / 2), _sizex / 2, _sizey / 2, _bgcolor);
            break;
        }
        }
        // Draw foreground (icon or text)
        if (_hasIcon)
        {
            _tft->drawXBitmap(_startx, _starty, _icon, _sizex, _sizey, _fgcolor);
        }
        else
        {
            _tft->setTextSize(_fontSize);
            _tft->setTextFont(_font);
            // Center the text (todo check if text is longer than button?)
            uint8_t fontSizeY = _tft->fontHeight();
            uint8_t fontSizeX = _tft->textWidth(_text, _font);
            uint16_t startX = _startx + ((_sizex - fontSizeX) / 2);
            uint16_t startY = _starty + ((_sizey - fontSizeY) / 2);
            // Print the text
            _tft->setCursor(startX, startY);
            _tft->print(_text);
        }
        // Draw tooltip
        if (_tooltipPresent)
        {
            _tft->setTextSize(_tooltipFontSize);
            _tft->setTextFont(_tooltipFont);
            uint8_t fontSizeX = _tft->textWidth(_text, _font);
            uint8_t fontSizeY = fontSizeY = _tft->fontHeight();
            uint8_t startX, startY;
            switch (_tooltipPosition)
            {
            case TooltipPositions::RIGHT:
            {
                // Center the text on the right
                startX = _startx + _sizex + _tooltipPadding;
                startY = _starty + ((_sizey - fontSizeY) / 2);
                break;
            }
            case TooltipPositions::LEFT:
            {
                // Center the text on the left
                startX = _startx - fontSizeX - _tooltipPadding;
                startY = _starty + ((_sizey - fontSizeY) / 2);
                break;
            }
            case TooltipPositions::UP:
            {
                // Center the text up
                startX = _startx + ((_sizex - fontSizeX) / 2);
                startY = _starty - _tooltipPadding;
                break;
            }
            case TooltipPositions::DOWN:
            {
                // Center the text down
                startX = _startx + ((_sizex - fontSizeX) / 2);
                startY = _starty + _sizey + _tooltipPadding;
                break;
            }
            }
            // Print the text
            _tft->setTextColor(_tooltipFgColor);
            _tft->setCursor(startX, startY);
            _tft->print(_tooltip);
        }
    }

    /**
     * @brief Checks for the pressing of the button
     *
     * @return true if the button is being pressed
     * @return false if the button is not being pressed
     */
    bool isPressed()
    {
        // If the touch occured inside the button's bounding box the press activated this button
        if (*_touchx > _startx && *_touchx<_startx + _sizex && * _touchy> _starty && *_touchy < _starty + _sizey)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    /**
     * @brief Sets the text of the button. Mutual alternative to setIcon.
     *
     * @param text: Text to be displayed
     * @param font: Font of the text
     * @param fontSize: Font size of the text
     */
    void setText(char *text, uint8_t font, uint8_t fontSize)
    {
        _hasIcon = false;
        _text = text;
        _font = font;
        _fontSize = fontSize;
    }

    /**
     * @brief Sets the icon of the button. Mutual alternative to setText.
     *
     * @param icon: Path to the icon to be displayed
     */
    void setIcon(uint8_t *icon)
    {
        _hasIcon = true;
        _icon = icon;
    }

    /**
     * @brief Get the X start cordinate
     *
     * @return uint16_t: Start x
     */
    uint16_t getStartX()
    {
        return _startx;
    }

    /**
     * @brief Get the X end cordinate
     *
     * @return uint16_t: End x
     */
    uint16_t getEndX()
    {
        return _startx + _sizex;
    }

    /**
     * @brief Get the Y start cordinate
     *
     * @return uint16_t: Start Y
     */
    uint16_t getStartY()
    {
        return _starty;
    }

    /**
     * @brief Get the Y end cordinate
     *
     * @return uint16_t: End Y
     */
    uint16_t getEndY()
    {
        return _starty + _sizey;
    }

    /**
     * @brief Get the X size
     *
     * @return uint16_t: X size
     */
    uint16_t getSizeX()
    {
        return _sizex;
    }

    /**
     * @brief Get the Y size
     *
     * @return uint16_t: Y size
     */
    uint16_t getSizeY()
    {
        return _sizey;
    }

    /**
     * @brief Sets the (optional) tooltip of a button
     *
     * @param tooltip: Tooltip text
     * @param font: Tooltip font
     * @param fontSize: Tooltip font size
     * @param position: Placement of the tooltip
     * @param padding: Tooltip padding (distance from buttonwidget)
     */
    void setTooltip(char *tooltip, uint8_t font, uint8_t fontSize, TooltipPositions position, uint16_t padding = 0, uint16_t fgcolor = TFT_BLACK)
    {
        _tooltipPresent = true;
        _tooltip = tooltip;
        _tooltipFont = font;
        _tooltipFontSize = fontSize;
        _tooltipPosition = position;
        _tooltipPadding = padding;
        _tooltipFgColor = fgcolor;
    }
};

#endif