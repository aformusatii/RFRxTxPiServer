#!/bin/bash
PLAYLIST_INDEX_FILE="paylist_index.dat"
PLAYLIST_INDEX=0

if [ -f $PLAYLIST_INDEX_FILE ];
then
   PLAYLIST_INDEX=$(cat $PLAYLIST_INDEX_FILE)
fi

if [ "$1" == "dec" ];
then
   PLAYLIST_INDEX=$((PLAYLIST_INDEX - 1))
else
   PLAYLIST_INDEX=$((PLAYLIST_INDEX + 1))
fi

if (( PLAYLIST_INDEX < 1 ));
then
    PLAYLIST_INDEX=9
fi

if (( PLAYLIST_INDEX > 9 ));
then
    PLAYLIST_INDEX=1
fi

echo -n $PLAYLIST_INDEX
echo -n $PLAYLIST_INDEX > $PLAYLIST_INDEX_FILE