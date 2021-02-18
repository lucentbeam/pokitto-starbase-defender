#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#ifndef POKITTO_SFML
#include "Pokitto.h"
#include "SDFileSystem.h"
#include "FATFileHandle.h"
#else
#include <fstream>
#endif

namespace Serialization {

#ifndef POKITTO_SFML

    SDFileSystem sd(/*MOSI*/P0_9, /*MISO*/P0_8, /*SCK*/P0_6, /*CS*/P0_7, /*Mountpoint*/"sd");

    FILE * fs_fopen( const char *file, const char *mode )
    {
        int flags = O_RDWR;
        if( mode[0] != 'r' )
        {
            flags |= O_CREAT;
            //if( mode[0] == 'w' )
            //    flags |= O_TRUNC;
            if( mode[0] == 'a' )
                flags |= O_APPEND;
        }
        else if( mode[1] == 0 )
        {
            flags = O_RDONLY;
        }
        return (FILE *) sd.open( file, flags );
    }

    // fclose
    int fs_fclose( FILE *fp )
    {
        if( fp )
            ((FATFileHandle*) fp)->close();
        return 0;
    }

    // fread
    unsigned int fs_fread (void *buffer, size_t a, size_t b, FILE *fp)
    {
        unsigned int r = ((FATFileHandle *) fp)->read( buffer, a*b );
        if( r == ~0UL ) r = 0;
        return r;
    }

    // fwrite
    unsigned int fs_fwrite (const void *buffer, size_t a, size_t b, FILE *fp)
    {
        return ((FATFileHandle *) fp)->write( buffer, a*b );
    }


    template<typename S>
    static bool tryStore(const char * fname, const S * data) {
        FILE *fp = fs_fopen(fname, "wb");
        if (fp == NULL) {
          return false;
        }
        fs_fwrite(data, 1, sizeof(S), fp);
        fs_fclose(fp);
        return true;
    }

    template<typename S>
    static bool tryGet(const char * fname, S * data) {
        FILE *fp = fs_fopen(fname, "rb");
        if (fp == NULL) {
            return false;
        }
        fs_fread(data, 1, sizeof(S), fp);
        fs_fclose(fp);
        return true;
    }

#else

    template<typename S>
    static bool tryStore(const char * fname, const S * data) {
        std::ofstream file;
        file.open(fname, std::ios::binary | std::ios::out);
        if (!file.is_open()) {
            return false;
        }
        file.write(reinterpret_cast<const char*>(data), sizeof(data));
        file.close();
        return true;
    }

    template<typename S>
    static bool tryGet(const char * fname, S * data) {
        std::ifstream file;
        file.open(fname, std::ios::binary | std::ios::in);
        if (!file.is_open()) {
            return false;
        }
        file.read(reinterpret_cast<char*>(data), sizeof(S));
        file.close();
        return true;
    }

#endif
};

#endif // SERIALIZATION_H
