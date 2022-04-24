
# To convert the images using ffmepg

ls | grep \.DAT | xargs -I xxx ffmpeg -vcodec rawvideo -f rawvideo -pix_fmt rgb565be -s 160x120 -i xxx -f image2 -vcodec png xxx.png
