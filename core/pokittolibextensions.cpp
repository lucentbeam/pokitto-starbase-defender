#include "pokittolibextensions.h"

#include <cstring>
#include <algorithm>

#ifndef POKITTO_SFML

#include "PokittoDisplay.h"

namespace Pokitto {

void DisplayExtensions::shift(int16_t x, int16_t y) {
    uint8_t* m_scrbuf = Display::getBuffer();
  for (int j = 0; j < 176; j++) {
      uint8_t * loc = m_scrbuf + 110 * j;
      std::memcpy(loc, loc+x, 110-x);
  }
}

void DisplayExtensions::drawTile(int16_t x, int16_t y, int16_t w, int16_t h, const uint8_t* tile) {
    if (y<-h || y>Display::height) return; //invisible
    if (x<-w || x>Display::width) return;  //invisible
    int screenh = std::min((int)h, Display::height- y);
    int screenw = std::min((int)w, Display::width - x);
    int screenx = x;
    int offsetx = 0;
    if (x < 0) {
        screenx = 0;
        offsetx = -x;
        screenw -= offsetx;
    }
    int screeny = y;
    int offsety = 0;
    if (y < 0) {
        screeny = 0;
        offsety = -y;
    }
    uint8_t* m_scrbuf = Display::getBuffer();
    for (int j = screeny; j < screeny+screenh; j++) {
      uint8_t * loc = m_scrbuf + 110 * j + screenx/2;
      const uint8_t * tloc = tile + w * (j - y)/2 + offsetx/2;
      std::memcpy(loc, tloc, screenw/2);
    }
}

void DisplayExtensions::drawClippedBitmap(int16_t x, int16_t y, int16_t w, int16_t h, int16_t clipw, int16_t cliph, const uint8_t* bitmap) {
  if (clipw == 0 || cliph == 0) {
      return;
  }
    int16_t scrx,scry,xjump,scrxjump;
    int16_t xclip;
    xclip=xjump=scrxjump=0;
    /** y clipping */
    if (y<0) { h+=y; bitmap -= y*(w>>1); y=0;}
    else if (y+h>Display::height) { h -=(y-Display::height);}
    /** x clipping */
    if (x<0) { xclip=(x&1)<<1; w+=x; xjump = ((-x)>>1); bitmap += xjump; x=0;}
    else if (x+w>Display::width) {
            xclip = (x&1)<<1;
            scrxjump = x&1;
            xjump=((x+w-Display::width)>>1)+scrxjump;
            w = Display::width-x;}

    uint8_t* scrptr = Display::getBuffer() + (y*(Display::width>>1) + (x>>1));
    /** ONLY 4-bit mode for time being **/
    for (scry = y; scry < y+h; scry+=1) {
        if ((scry - y) > cliph) return;
        if (scry>=Display::height) return;
        if ((x&1)==0) { /** EVEN pixel starting line, very simple, just copypaste **/
            for (scrx = x; scrx < w+x-xclip; scrx+=2) {
                if ((scrx-x) > clipw) {
                  bitmap++;
                  scrptr++;
                  continue;
                }
                uint8_t sourcepixel = *bitmap;
                if (xclip) {
                  sourcepixel <<=4;
                  sourcepixel |= ((*(bitmap+1))>>4);
                }
                uint8_t targetpixel = *scrptr;
                if ((sourcepixel>>4) != Display::invisiblecolor ) targetpixel = (targetpixel&0x0F) | (sourcepixel & 0xF0);
                if ((sourcepixel&0x0F) != Display::invisiblecolor) targetpixel = (targetpixel & 0xF0) | (sourcepixel & 0x0F);
                *scrptr = targetpixel;
                bitmap++;
                scrptr++;
            }
            if (xclip){
                if (w&1) {
                    /**last pixel is odd pixel due to clipping & odd width*/
                    uint8_t sourcepixel = *bitmap;
                    if ((sourcepixel&0x0F) != Display::invisiblecolor) {
                        sourcepixel <<=4;
                        volatile uint8_t targetpixel = *scrptr;// & 0x0F;
                        targetpixel &= 0xF; //clear upper nibble
                        targetpixel |= sourcepixel; //now OR it
                        *scrptr = targetpixel;
                    }
                    //scrptr++;
                }
                bitmap++;
                scrptr++;
            }
            bitmap += xjump; // needed if x<0 clipping occurs
        } else { /** ODD pixel starting line **/
            uint8_t sourcepixel;
            uint8_t targetpixel;
            for (scrx = x; scrx < w+x-xclip; scrx+=2) {
                if ((scrx-x) > clipw) {
                  bitmap++;
                  scrptr++;
                  continue;
                }
                sourcepixel = *bitmap;
                targetpixel = *scrptr;
                // store higher nibble of source pixel in lower nibble of target
                if((sourcepixel>>4)!=Display::invisiblecolor) targetpixel = (targetpixel & 0xF0) | (sourcepixel >> 4 );
                *scrptr = targetpixel;
                scrptr++;
                targetpixel = *scrptr;
                // store lower nibble of source pixel in higher nibble of target
                if((sourcepixel&0x0F)!=Display::invisiblecolor) targetpixel = (targetpixel & 0x0F) | (sourcepixel << 4);
                *scrptr = targetpixel;
                bitmap++;
            }
            if (xclip) {
                // last line, store higher nibble of last source pixel in lower nibble of last address
                targetpixel = *scrptr;
                sourcepixel = *bitmap >> 4;
                if(sourcepixel!=Display::invisiblecolor) targetpixel = (targetpixel & 0xF0) | sourcepixel;
                *scrptr = targetpixel;
            }
            bitmap+=xjump;
        }
        // increment the y jump in the scrptr
        scrptr = scrptr + ((Display::width - w)>>1)+scrxjump;
    }
}

void DisplayExtensions::drawBitmap(int16_t x, int16_t y, const uint8_t* bitmap, uint8_t frame, bool flip)
{
    int16_t w = *bitmap;
    int16_t h = *(bitmap + 1);
    /** visibility check */
    if (y<-h || y>Display::height) return; //invisible
    if (x<-w || x>Display::width) return;  //invisible
    bitmap = bitmap + 2 + frame * uint8_t(float(w)/2.0f + 0.5f) * h ;
    if (flip) {
        Display::drawBitmapDataXFlipped(x, y, w, h, bitmap);
    } else {
      Display::drawBitmapData(x, y, w, h, bitmap);
    }
}

}

#include "SoftwareI2C.h"

#include <MemOps>

#pragma GCC diagnostic ignored "-Wattributes"
#define NAKED __attribute__((naked))

namespace Audio {

    void getVolume(uint8_t* buf){
        SoftwareI2C(P0_4, P0_5).read(0x5e, buf, 1);
    }
}

#endif
