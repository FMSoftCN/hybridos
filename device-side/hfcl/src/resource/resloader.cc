/* 
** HFCL - HybridOS Foundation Class Library
** 
** Copyright (C) 2018 Beijing FMSoft Technologies Co., Ltd.
**
** This file is part of HFCL.
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "resource/respkgmanager.h"
#include "resloader.h"
#include "restypes.h"

//#include "mci.h"
#include "resource_audio.h"

extern INNER_RES_ARRAY g_ResArray[];
extern int g_ResCount; 
#ifdef ENABLE_MCI
extern void InitRingRes();
#endif  // ENABLE_MCI

namespace hfcl {

INNER_RES_INFO* GetImageResInfo(const char *filepath)
{
    int i,j;
    int cont;
    INNER_RES_INFO* info = NULL;

    for(i = 0; i < g_ResCount; i++) {
        info = g_ResArray[i].pInner_res_info;
        cont = g_ResArray[i].number;
        for(j = 0; j < cont; j++) {
            if(info[j].res_name != NULL && strcmp(filepath, info[j].res_name) == 0) {
                return &info[j];
            }
        }
    }

    return NULL;
}

ResLoader* ResLoader::m_singleton = NULL;

ResLoader::ResLoader()
{
}

ResLoader* ResLoader::getInstance()
{
    if (NULL == ResLoader::m_singleton)
        ResLoader::m_singleton = HFCL_NEW_EX(ResLoader, ());

    return ResLoader::m_singleton;
}

#ifdef USE_RDA_FONT
extern "C" Logfont        MMI_DEFAULT_FONT;
extern "C" Logfont        MMI_SMALL_FONT;
extern "C" Logfont        MMI_MEDIUM_FONT;
extern "C" Logfont        MMI_MEDIUM_BOLD_FONT;
extern "C" Logfont      MMI_LARGE_FONT;
extern "C" Logfont      MMI_DIALER_FONT1;
extern "C" Logfont      MMI_DIALER_FONT2;
extern "C" Logfont      MMI_DIALER_FONT3;
extern "C" Logfont        MMI_SUBLCD_FONT;
extern "C" Logfont        MMI_VIRTUAL_KEYBOARD_FONT;
extern "C" Logfont        MMI_IDLETIME_FONT;

Logfont* ResLoader::getFont(const char* fontname/*, Style* style = NULL*/)
{
    if (fontname != NULL)
    {
        if (strcmp(fontname, "small font") == 0){
            return (&MMI_SMALL_FONT);
        } else if (strcmp(fontname, "medium font") == 0){
            return (&MMI_VIRTUAL_KEYBOARD_FONT);
        } else if (strcmp(fontname, "medium bold font") == 0){
            return (&MMI_MEDIUM_FONT);
        } else if (strcmp(fontname, "large font") == 0){
            return (&MMI_LARGE_FONT);
        } else if (strcmp(fontname, "dial font 1") == 0){
            return (&MMI_DIALER_FONT1);
        } else if (strcmp(fontname, "dial font 2") == 0){
            return (&MMI_DIALER_FONT2);
        } else if (strcmp(fontname, "time font") == 0){
            return (&MMI_SUBLCD_FONT);
        } else if (strcmp(fontname, "idletime font") == 0){
            return (&MMI_IDLETIME_FONT);
        }         
    }
    return (&MMI_DEFAULT_FONT);
}

bool ResLoader::releaseFont(const char* fontname)
{
    return true;
}

#else

Logfont* ResLoader::getFont(const char* fontname/*, Style* style = NULL*/)
{
    string fontName(fontname);
    FontResMap::iterator it = m_fontRes.find(fontName);

    if (it == m_fontRes.end()) {
        Font* font = Font::createFont(fontname);
        if(!font)
            return NULL;
        m_fontRes[fontname] = font;
        return font->getLogfont();
    }

    return it->second->getLogfont();
}

bool ResLoader::releaseFont(const char* fontname)
{
    string st(fontname);
    FontResMap::iterator it = m_fontRes.find(st);

    if (it == m_fontRes.end())
        return true;

    it->second->unref();

    if (0 == it->second->getRefCnt()) {
        m_fontRes.erase(it);
    }

    return true;
}

#endif

void ResLoader::registerInnerRes(int res_type, INNER_RES_INFO * resources, int count)
{
    if (res_type != R_TYPE_IMAGE || resources == NULL || count <= 0)
        return;

    for (int i=0; i < count; i++) {
        if (resources[i].res_name != NULL && resources[i].data != NULL && resources[i].size >= 0) {
            InnerImage & inner = m_innerImageRes[resources[i].res_name];
            inner.resInfo = &resources[i];
        }
    }
}

GifAnimate* ResLoader::getGifAnimate(const char* filepath)
{
    GifAnimate* gif = NULL;
    INNER_RES_INFO* info = NULL;

    // GifAnimate : avoid mem leak TODO
    gif = HFCL_NEW_EX(GifAnimate, ());
    info = GetImageResInfo(filepath);

    if (info != NULL) {
#ifdef _HFCL_INCORE_BMPDATA
        gif->createGifAnimateFromMem((const char*)info->data, info->size);
#else
        gif->createGifAnimateFromRes((BitmapFrameArray *) info->data);
#endif
    }
    else {
        // 2. file system
        gif->createGifAnimateFromFile(filepath);
    }

    gif->safeRef();
    return gif;
}

Image* ResLoader::getImage(const char* filepath)
{
#if 0
    Image* image = NULL;
    INNER_RES_INFO* info = NULL;

    info = GetImageResInfo(filepath);
    if(info != NULL)
       {
            image = Image::loadImage(info->res_name, (const char*)info->data, info->size);
          if(image != NULL) 
          {
            image->safeRef();
            return image;
          }
    }

    ImageResMap::iterator it = m_imageRes.find(filepath);
    if (it != m_imageRes.end()) {
        it->second->safeRef();
        return it->second;
    }

    image = Image::loadImage(filepath);
    if(NULL != image){
        image->safeRef();
        m_imageRes[filepath] = image;
    }

    return image;
#else 
    return Image::loadImage(filepath);
#endif
}

bool ResLoader::releaseImage(const char* filepath)
{
    InnerImageResMap::iterator iit = m_innerImageRes.find(filepath);
    if (iit != m_innerImageRes.end() && iit->second.image != NULL){
        iit->second.image->safeUnref();
        iit->second.image = NULL;
        return true;
    }

    ImageResMap::iterator it = m_imageRes.find(filepath);
    if (it != m_imageRes.end() && it->second != NULL){
        it->second->safeUnref();
        m_imageRes.erase(it);
        return true;
    }

    return false;
}

Bitmap* ResLoader::getBitmap(const char* filename)
{
    Bitmap *pbmp = NULL;
    INNER_RES_INFO* info = NULL;
    
    info = GetImageResInfo(filename);
    if (NULL != (pbmp = HFCL_NEW_EX(Bitmap, ()))) {
        if (NULL != info) {
            const char * externs_name = NULL;

            externs_name = strrchr(filename, '.');
            if (!externs_name) {
                HFCL_DELETE(pbmp);
                return NULL;
            }

            externs_name ++;

            if (GraphicsContext::screenGraphics()->loadBitmap(&pbmp, info->data, info->size, externs_name)) {
                _DBG_PRINTF ("ResLoader::getBitmap: Failed to load image from memory: %s", filename);
                HFCL_DELETE(pbmp);
                return NULL;
            }
        }
        else {
            if (GraphicsContext::screenGraphics()->loadBitmap(pbmp, filename)) {
                _DBG_PRINTF ("ResLoader::getBitmap: Failed to load image: %s", filename);
                HFCL_DELETE(pbmp);
                return NULL;
            }
        }
    }

    return pbmp;
}

bool ResLoader::releaseBitmap(const char* filepath)
{
    BitmapResMap::iterator it = m_bitmapRes.find(filepath);

    if (it == m_bitmapRes.end()){
        return false;
    }

    GraphicsContext::screenGraphics()->unloadBitmap(it->second);
    HFCL_DELETE(it->second);
    m_bitmapRes.erase(it);

    return true;
}

bool ResLoader::releaseBitmap(Bitmap* pbmp)
{
    BitmapResMap::iterator it = m_bitmapRes.begin();
    while (it != m_bitmapRes.end()) {
        if (it->second == pbmp){
            GraphicsContext::screenGraphics()->unloadBitmap(it->second);
            HFCL_DELETE(it->second);
            m_bitmapRes.erase(it);
            return true;
        }
        ++it;
    }
    return false; 
}

BitmapFrameArray* ResLoader::InnerImage::getBitmapFrame()
{
    // BitmapFrame * !!!
    return (BitmapFrameArray *)resInfo->data;
}

Image* ResLoader::InnerImage::get()
{
    if(!image) {
        image = Image::loadImage(resInfo->res_name);
    }
    return image;
}

const char ** ResLoader::InnerImage::getBitmapdata()
{
    return (const char * *)resInfo->data;
}

unsigned int ResLoader::InnerImage::getBitmapdatasize()
{
    return (unsigned int)resInfo->size;
}

INNER_RES_INFO* ResLoader::InnerImage::getResInfo(void)
{
    return resInfo;
}

void RegisterInnerResource(int type, INNER_RES_INFO *resource, int count)
{
    ResLoader::getInstance()->registerInnerRes(type, resource, count);
}

extern "C"
{
    device_tones_struct **resource_tones = NULL;
    audio_resource_struct *resource_midis = NULL;
    audio_resource_struct *resource_sounds = NULL;
    audio_resource_struct *resource_ems_sounds = NULL;
    audio_resource_struct *resource_imelodys = NULL;
    audio_resource_struct *resource_ems_imelodys = NULL;
    audio_resource_struct *resource_message_sounds = NULL;
    audio_resource_struct* resource_poweronoff_sounds = NULL;
    audio_resource_struct* resource_mms_sounds = NULL;
    audio_resource_struct* resource_fng_sounds = NULL;   //Added by jinzh:20070724
    audio_resource_struct* resource_alarm_sounds = NULL;
    audio_resource_struct* resource_camera_sounds = NULL;   

#ifdef DIGIT_TONE_SUPPORT
    audio_resource_struct *resource_human_voice_tones = NULL;
#endif 
#ifdef SIMPLE_TTS
    audio_resource_struct *resource_tts_sounds = NULL;
    start_end_t *TTS_UCS2_MSB_index = NULL;
    unsigned char *TTS_UCS2_LSB = NULL;
    unsigned short *TTS_index_table = NULL;
#endif /* SIMPLE_TTS */ 
#ifdef CUST_KEYPAD_TONE_SUPPORT
    audio_resource_struct *resource_keypad_tones = NULL;
#endif
    audio_resource_struct* resource_answering_machine_greeting_voice = NULL;  
}

extern "C"
{
    extern const audio_resource_struct CSD_resource_midis[];
    extern const audio_resource_struct CSD_resource_sounds[];
    extern const audio_resource_struct CSD_resource_poweronoff_sounds[];
    extern const audio_resource_struct CSD_resource_imelodys[];
    extern const audio_resource_struct CSD_resource_message_sounds[];
#if defined(__MMI_GAME__)
    extern const audio_resource_struct CSD_resource_fng_sounds[];   
#endif
    extern const audio_resource_struct CSD_resource_alarm_sounds[];
    extern const audio_resource_struct CSD_resource_camera_sounds[];
#ifdef DIGIT_TONE_SUPPORT
    extern const audio_resource_struct CSD_resource_human_voice_tones[];
#endif
#ifdef SIMPLE_TTS
    extern const audio_resource_struct CSD_resource_tts_sounds[];
    extern const start_end_t CSD_TTS_UCS2_MSB_index[];
    extern const unsigned char CSD_TTS_UCS2_LSB[];
    extern const unsigned short CSD_TTS_index_table[];
#endif
#ifdef CUST_KEYPAD_TONE_SUPPORT
    extern const audio_resource_struct CSD_resource_keypad_tones[];
#endif
    extern const audio_resource_struct CSD_resource_answering_machine_greeting_voice[];
}

void RegisterAudiorResource()
{
#ifdef ENABLE_MCI
      InitRingRes();
      resource_sounds = (audio_resource_struct*) CSD_resource_sounds;
      resource_poweronoff_sounds = (audio_resource_struct*) CSD_resource_poweronoff_sounds;
      resource_imelodys = (audio_resource_struct*) CSD_resource_imelodys;
      resource_message_sounds = (audio_resource_struct*) CSD_resource_message_sounds;
#if defined(__MMI_GAME__)
      resource_fng_sounds = (audio_resource_struct*) CSD_resource_fng_sounds;
#endif
      resource_alarm_sounds = (audio_resource_struct*) CSD_resource_alarm_sounds;
#ifdef CAMERA_MODULE
     resource_camera_sounds = (audio_resource_struct*) CSD_resource_camera_sounds;
#endif
#ifdef DIGIT_TONE_SUPPORT
      resource_human_voice_tones = (audio_resource_struct*) CSD_resource_human_voice_tones;
#endif
#ifdef SIMPLE_TTS
      resource_tts_sounds = (audio_resource_struct*) CSD_resource_tts_sounds;
      TTS_UCS2_MSB_index = (start_end_t*) CSD_TTS_UCS2_MSB_index;
      TTS_UCS2_LSB = (unsigned char*) CSD_TTS_UCS2_LSB;
      TTS_index_table = (unsigned short*) CSD_TTS_index_table;
#endif
#ifdef CUST_KEYPAD_TONE_SUPPORT
      resource_keypad_tones = (audio_resource_struct*) CSD_resource_keypad_tones;
#endif
     resource_answering_machine_greeting_voice = (audio_resource_struct*) CSD_resource_answering_machine_greeting_voice;
#endif // ENABLE_MCI
}


unsigned int GetAudioResBuffer(unsigned char audioId , unsigned int** pBuffer ,unsigned int* codec)
{
    unsigned int length = 0;
     
    //sms
    if ((MIN_MSG_SND_ID <= audioId) && (MAX_MSG_SND_ID >= audioId))
    {

        *pBuffer =  (unsigned int*)resource_message_sounds[audioId - MIN_MSG_SND_ID].data;
        length = resource_message_sounds[audioId - MIN_MSG_SND_ID].len;
        length /= 4;
        *codec = resource_message_sounds[audioId - MIN_MSG_SND_ID].format;;
        return  length;
    }
       //alarm
    else if ((MIN_ALARM_SND_ID <= audioId) && (MAX_ALARM_SND_ID >= audioId))
    {

        *pBuffer =  (unsigned int*)resource_alarm_sounds[audioId - MIN_ALARM_SND_ID].data;
        length = resource_alarm_sounds[audioId - MIN_ALARM_SND_ID].len;
        length /= 4;
        *codec = resource_alarm_sounds[audioId - MIN_ALARM_SND_ID].format;;
        return  length;
    }
    //call
    else if ((MIN_RING_TONE_ID <= audioId) && (MAX_RING_TONE_ID >= audioId))
    {    

        *pBuffer =  (unsigned int*)resource_imelodys[audioId - MIN_RING_TONE_ID].data;
        length = resource_imelodys[audioId - MIN_RING_TONE_ID].len;
        length /= 4;
        *codec =  resource_imelodys[audioId - MIN_RING_TONE_ID].format;
        return  length;
    }
    //CALL
    else if ((MIN_MIDI_ID <= audioId) && (MAX_MIDI_ID >= audioId))
    {

        *pBuffer =  (unsigned int*)resource_sounds[audioId - MIN_MIDI_ID].data;
        length = resource_sounds[audioId - MIN_MIDI_ID].len;
        length /= 4;
        *codec = resource_sounds[audioId - MIN_MIDI_ID].format;;
        return  length;
    }
#if defined(__MMI_GAME__)
    // Fun and Game
    else if ((MIN_FNG_SND_ID <= audioId) && (MAX_FNG_SND_ID >= audioId))
    {
        

        *pBuffer =  (unsigned int*)resource_fng_sounds[audioId - MIN_FNG_SND_ID].data;
        length = resource_fng_sounds[audioId - MIN_FNG_SND_ID].len;
        length /= 4;
        *codec = resource_fng_sounds[audioId - MIN_FNG_SND_ID].format;
        return  length;
    }                        
#endif    
    //camera
    else if ((MIN_CAMERA_SND_ID <= audioId) && (MAX_CAMERA_SND_ID >= audioId)) 
    {
        

        *pBuffer =  (unsigned int*)resource_camera_sounds[audioId - MIN_CAMERA_SND_ID].data;
        length = resource_camera_sounds[audioId - MIN_CAMERA_SND_ID].len;
        length /= 4;

        *codec = resource_camera_sounds[audioId - MIN_CAMERA_SND_ID].format;
        return  length;
    }    
    //power on / off
    else if ((MIN_SND_ID <= audioId) && (MAX_SND_ID >= audioId))
    {
        
        *pBuffer =  (unsigned int*)resource_poweronoff_sounds[audioId - MIN_SND_ID].data;
        length = resource_poweronoff_sounds[audioId - MIN_SND_ID].len;
        *codec = resource_poweronoff_sounds[audioId - MIN_SND_ID].format;


        length /= 4;
        return  length;
    }
    //answering machine greeting voice
    else if ((MIN_GREETING_VOICE_ID <= audioId) && (MAX_GREETING_VOICE_ID >= audioId))
    {
        *pBuffer =  (unsigned int*)resource_answering_machine_greeting_voice[audioId - MIN_GREETING_VOICE_ID].data;
        length = resource_answering_machine_greeting_voice[audioId - MIN_GREETING_VOICE_ID].len;
        *codec = resource_answering_machine_greeting_voice[audioId - MIN_GREETING_VOICE_ID].format;
        length /= 4;
        return  length;
    }
#ifdef DIGIT_TONE_SUPPORT
    else if ((MIN_HUMAN_VOICE_ID <= audioId) && (MAX_HUMAN_VOICE_ID >= audioId))
    {
        
        *pBuffer =  (unsigned int*)resource_human_voice_tones[audioId - MIN_HUMAN_VOICE_ID].data;
        length = resource_human_voice_tones[audioId - MIN_HUMAN_VOICE_ID].len;
        *codec = resource_human_voice_tones[audioId - MIN_HUMAN_VOICE_ID].format;

        length /= 4;
        return  length;
    }    
#endif    
        else //used for setting volume to play the music
    {
        *pBuffer =  (unsigned int*)resource_imelodys[0].data;
        length = resource_imelodys[0].len;
        length /= 4;
        *codec = resource_imelodys[0].format;

        return  length;
    }
}

} // namespace hfcl

