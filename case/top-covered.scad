/*
 * @copyright Copyright (C) 2016 Leiden Tech. All rights reserved.
 * @license http://www.gnu.org/licenses/gpl-3.0.txt GNU General Public License v3
 * 
 */
//covered switches boloblinky variation

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
 translate([$length-$wall*2-17,$wall*2+1,0])  
 cube ([16, 8, $wall]);
 translate([$length-$wall*2-17,$width-11,0])  
 cube ([16, 8, $wall]);

 //usb hole
 translate([$wall-2,$width-11,5.5+$wall])  
 cube ([5, 5, 9]);
}
//button top cutout fill  
translate([$length-$wall*2-17,$wall*3+1,0])  
cube ([15, 6, $wall]);
translate([$length-$wall*2-17,$width-5-5,0])  
cube ([15, 6, $wall]);

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

//buttons housing
translate([$length-$wall*2-19,$wall+10.5,0])  
cube ([18, $wall*2, 10]);

translate([$length-$wall*2-19,$wall+1.5,4])
difference (){
 cube ([18, 10, 6]);
 translate([2,.9,0])  
 cube ([13.1, 6.3, 6]);
}

translate([$length-$wall*2-19,$width-13.2,0])  
cube ([18, $wall*2, 10]);

translate([$length-$wall*2-19,$width-12,4])  
difference (){
 cube ([18, 9, 6]);
 translate([2,2.2,0])  
 cube ([13.1, 6.3, 6]);
}
//end button housing

//nano end wall
difference() {
 translate([$wall*2+43,$width-10,$wall])  
 cube ([4, 7, 12]);
 translate([$wall*2+43,$width-7,$wall])  
 cube ([2, 3, 12]);
}
