@echo off
title Render Animation Series

for %%f in ("level_set_liquid_sim_output\*") do (
echo ">> frame %%~nf start"
mitsuba -qx -r 5 -Dframe_name=%%~nf -o %%~nf.png  dam_breaking.xml
echo ">> frame %%~nf complete"
)

ffmpeg -r 30 -f image2 -s 1024x1024 -start_number 0 -i frame_%06d.png -vframes 100 -vcodec libx264 -crf 25 -pix_fmt yuv420p dam_breaking.mp4

echo all complete

pause