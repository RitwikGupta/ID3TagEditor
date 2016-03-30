#include <stdio.h>
#include <string.h>
/* For atoi (can't have implicit declaration in C99) */
#include <stdlib.h>

/* ID3 tag struct */
struct Tag {
    char title[30];
    char artist[30];
    char album[30];
    char year[4];
    char comment[28];
    char zero_sep;
    char track_num;
    char genre;
};

/*
 * argv[1] is the FILENAME
 * argv[2] is the TAG to modify
 */
int main( int argc, const char *argv[] )
{
    char tag_to_edit[8] = { 0 };
    /* Extra element in id to add the null terminator */
    char id[4] = { 0 };
    struct Tag tag = {  {0} };
    FILE *fp;
    int edit_arg_int;

    fp = fopen(argv[1], "r+b");
    if(fp == NULL)
    {
        perror("Error: ");
        return -1;
    }

    /* Get the tag to edit */
    edit_arg_int = 2;
    while(edit_arg_int < argc || argc == 2) {
        if (argc > 3)
        {
            strncpy(tag_to_edit, argv[edit_arg_int], 8);
        }

        /* Seek to the last 128 bytes of the file */
        fseek(fp, -128L, SEEK_END);
        fread(id, 3, 1, fp);

        /* Read in entire ID3 tag after TAG, element by element */
        fread(&tag, sizeof( struct Tag ), 1, fp);
        id[3] = 0;
        tag.zero_sep = 0;
        tag.genre = atoi(&tag.genre);

        /* If the is no TAG, set the TAG field to NON */
        if (strncmp("TAG", id, 3) != 0)
        {
            strncpy(id, "NON", 3);
            tag = (const struct Tag){ {0} };
            fclose(fp);
            fopen(argv[1], "a+b");
        }

        /* Should be able to edit the following in lowercase:
         * ARTIST
         * TITLE
         * ALBUM
         * YEAR
         * COMMENT
         * TRACK
         */
        if (strlen(tag_to_edit) > 0)
        {
            strncpy(id, "TAG", 3);
            if (strncmp("-title", tag_to_edit, 6) == 0)
            {
                strncpy(tag.title, "0", 30);
                strncpy(tag.title, argv[edit_arg_int + 1], 30);
            }
            else if (strncmp("-artist", tag_to_edit, 7) == 0)
            {
                strncpy(tag.artist, "0", 30);
                strncpy(tag.artist, argv[edit_arg_int + 1], 30);
            }
            else if (strncmp("-album", tag_to_edit, 6) == 0)
            {
                strncpy(tag.album, "0", 30);
                strncpy(tag.album, argv[edit_arg_int + 1], 30);
            }
            else if (strncmp("-year", tag_to_edit, 5) == 0)
            {
                strncpy(tag.year, "0", 4);
                strncpy(tag.year, argv[edit_arg_int + 1], 4);
            }
            else if (strncmp("-comment", tag_to_edit, 8) == 0)
            {
                strncpy(tag.comment, "0", 28);
                strncpy(tag.comment, argv[edit_arg_int + 1], 28);
            }
            else if (strncmp("-track", tag_to_edit, 6) == 0)
            {
                tag.track_num = 0;
                tag.track_num = atoi(argv[edit_arg_int + 1]);
            } else {
                break;
            }
        }

        /* Write the tag to the last 128 bytes */
        if (strncmp("TAG", id, 3) == 0 && argc > 3)
        {
            fseek(fp, -128L, SEEK_END);
            fwrite(&id, sizeof(id) - 1, 1, fp);
            fseek(fp, -125L, SEEK_END);
            fwrite(&tag, sizeof(tag), 1, fp);
        }
        if(argc == 2) {
            break;
        }
        edit_arg_int += 2;
    }
    if (strncmp("TAG", id, 3) == 0)
    {
        printf("The TAG is: [%s]\n", id);
        printf("The TITLE is: [%s]\n", tag.title);
        printf("The ALBUM is: [%s]\n", tag.album);
        printf("The YEAR is: [%.4s]\n", tag.year);
        printf("The COMMENT is: [%s]\n", tag.comment);
        printf("The ARTIST is: [%s]\n", tag.artist);
        printf("The ZERO_SEP is: [%c]\n", tag.zero_sep);
        printf("The TRACK_NUM is: [%d]\n", tag.track_num);
        printf("The GENRE is: [%d]\n", tag.genre);
    }
    else
    {
        printf("There is no tag in this music file.\n");
    }

    fclose(fp);

    return 0;
}
