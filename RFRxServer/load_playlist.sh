mpc clear
case $1 in
    1) ./speech.sh "Stream 2016" "true"; mpc add "youtube/Stream-2016";;
    2) ./speech.sh "Stream 2011" "true"; mpc add "youtube/Stream-2011";;
    3) ./speech.sh "Chillout Music" "true"; mpc add "youtube/Chillout";;
    4) ./speech.sh "Classic Music" "true"; mpc add "youtube/Classic";;
    5) ./speech.sh "RAP Music" "true"; mpc add "youtube/RAP";;
    6) ./speech.sh "RAP bit" "true"; mpc add "youtube/RapBit";;
    7) ./speech.sh "Trance" "true"; mpc add "youtube/Trance";;
    8) ./speech.sh "Rap 2015" "true"; mpc add "RAP-2015";;
    9) ./speech.sh "Internet Radio" "true" & mpc load "radio";;
    *) ./speech.sh "Invalid Number" "true"; echo "INVALID NUMBER -> " $1 ;;
esac
mpc play