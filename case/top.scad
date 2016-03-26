/*
 * @copyright Copyright (C) 2016 Leiden Tech. All rights reserved.
 * @license http://www.gnu.org/licenses/gpl-3.0.txt GNU General Public License v3
 * 
 */
//naked switches boloblinky

$wall=1;
$height=15;
$length=71;
$width=60;
$totalheight=20;
$screen=32;

difference (){
 translate([0,0,0])  
 cube ([$length, $width, $height]);

 //cutout
 translate([$wall*1.5,$wall*1.5,$wall]) 
 cube ([$length-$wall*3, $width-$wall*3, $totalheight]);

 //screen
 translate([5+$wall*2,($width-$screen)/2,0])
 cube ([$screen, $screen, $wall]); 

 //button top cutout
 translate([$length-$wall*2-14,$wall*2+1.5,0])  
 cube ([13.1, 6.3, $wall]);
 translate([$length-$wall*2-14,$width-$wall*2-7.5,0])  
 cube ([13.1, 6.3, $wall]);
 //usb hole
 translate([$wall-2,$width-11,5.5+$wall])  
 cube ([5, 5, 9]);
}

difference()
{ //totalheight box
 translate([$wall*2,$wall*2,$wall])  
 cube ([$length-$wall*3-1, $width-$wall*3-1, $totalheight]);
 //inner cutout
 translate([$wall*3,$wall*3,$wall])  
 cube ([$length-$wall*4-2, $width-$wall*4-2, $totalheight]);

 //usb hole
 translate([$wall-2,$width-11,5.5+$wall])  
 cube ([5, 5, 9]); 
}
//nano align
translate([$wall+18,$width-8,$wall+.4])  
cube ([10, 1, 1]);

//max7219 pillow
difference()
{
 translate([$wall*2,$width/2-9,$wall])  
 cube ([5, 18, 8]);
 translate([$wall*2,$width/2-7,3.5])  
 cube ([5, 14, 8]);
}
 
//nano end wall
difference() {
 translate([$wall*2+43,$width-10,$wall])  
 cube ([4, 7, 12]);
 translate([$wall*2+43,$width-7,$wall])  
 cube ([2, 3, 12]);
}

//button housing
difference()
{
  translate([$length-$wall*3-14,$wall+1,0])  
  cube ([14.1+$wall, 6.3+$wall*2, 6-$wall]);
  translate([$length-$wall*2-14,$wall*2+1,0])  
  cube ([13.1, 6.3, 8]);
}
difference()
{
 translate([$length-$wall*3-14,$width-$wall*3-7.5,0]) 
 cube ([14.1+$wall, 6.3+$wall*2, 5.5-$wall]);
 translate([$length-$wall*2-14,$width-$wall*2-7.5,0]) 
 cube ([13.1, 6.3, 8]);
}
