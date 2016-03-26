/*
 * @copyright Copyright (C) 2016 Leiden Tech. All rights reserved.
 * @license http://www.gnu.org/licenses/gpl-3.0.txt GNU General Public License v3
 * 
 * The Write.scad library can be found at http://www.thingiverse.com/thing:16193 or https://github.com/rohieb/Write.scad.git 
 */
use <Write.scad/Write.scad>
$slotsize=1.1;
$outsidewall=2;
$wall=2;
$height=13;
$length=71;
$totalheight=20;
center=false;
//AA
$l = 50.5; $w = 14.5;$gap=.5;
t=3;
      
difference(){ //outside wall ends
//bigbox
cube ([$length, $l+$wall*2+$slotsize*2+$outsidewall*2, $totalheight]);
//cutout
translate([$outsidewall,$outsidewall,$height+$wall])     
cube ([$length-$outsidewall*2, $l+$wall*2+$slotsize*2, $totalheight-$height]);
//slide switch
translate([$length-$outsidewall-3,25,$height/2])    
    cube ([8,9.3,5.5]);
    
rotate([90,0,270])
   translate([-11.1,15,-69.5])
   write("+",t=t-2,h=$height/4+2,center=center);
rotate([90,0,270])
   translate([-17,15,-69.5])
   write("-",t=t-2,h=$height/4+2,center=center);
    
rotate([90,0,90])
   translate([24,13.5,70])
   write("1",t=t-2,h=$height/4+2,center=center);
rotate([90,0,90])
   translate([32,13.5,70])
   write("0",t=t-2,h=$height/4+2,center=center);

//wires
translate([$wall,$wall,$height])    
    cube ([$length-$wall*2,2,2]); 
translate([$length-$wall*2-1,$wall*2,$height])    
    cube ([3,3,2]); 
translate([$length-$wall*2-3,$wall,$height-$wall])    
    cube ([5,2.1,6]);
translate([$length-$wall*2-1,$wall*2,$height-$wall])    
    cube ([3,2,2]);
//1
$index=2;
echo ($index);
translate([$index,$wall*1.5+$slotsize+$outsidewall/2,$wall])     
cube ([$w+$gap, $l, $height]);

translate([$index+$wall*2,$wall+$outsidewall/2,$wall])     
cube ([8, $l+$wall*2, $height]);


translate([$index+$wall,$wall+$outsidewall/2,$wall])     
cube ([12, $slotsize, $height]);
    
translate([$index+$wall*1.5,$l+$wall*2+$slotsize+$outsidewall/2,$wall])     
cube ([13, $slotsize, $height]);  
  
    
//2
$index2=18;
echo ($index2);
translate([$index2,$wall*1.5+$slotsize+$outsidewall/2,$wall])     
cube ([$w+$gap, $l, $height]);

translate([$index2+$wall*2,$wall+$outsidewall/2,$wall])     
cube ([8, $l+$wall*2, $height]);

translate([$index2+$wall*1.5,$wall+$outsidewall/2,$wall])     
cube ([13, $slotsize, $height]);
    
translate([$index2,$l+$wall*2+$slotsize+$outsidewall/2,$wall])     
cube ([13, $slotsize, $height]);  


//3
$index3=34;
echo ($index3);
translate([$index3,$wall*1.5+$slotsize+$outsidewall/2,$wall])     
cube ([$w+$gap, $l, $height]);

translate([$index3+$wall*2,$wall+$outsidewall/2,$wall])     
cube ([8, $l+$wall*2, $height]);

translate([$index3,$wall+$outsidewall/2,$wall])     
cube ([13, $slotsize, $height]);
    
translate([$index3+$wall*1.5,$l+$wall*2+$slotsize+$outsidewall/2,$wall])
cube ([13, $slotsize, $height]);  

//4
$index4=50;
echo ($index4);
translate([$index4,$wall*1.5+$slotsize+$outsidewall/2,$wall])
//battery
cube ([$w+$gap, $l, $height]);
//end cutout
translate([$index4+$wall*2,$wall+$outsidewall/2,$wall])     
cube ([8, $l+$wall*2, $height]);
 //left end    
translate([$index4+$wall,$wall+$outsidewall/2,$wall])
cube ([12, $slotsize, $height]);
//right end
translate([$index4,$l+$wall*2+$slotsize+$outsidewall/2,$wall])
cube ([13, $slotsize, $height]);      
       
//extra slot
$index5=66;
translate([$index5,$wall*1.5+$slotsize+$outsidewall/2,$wall])     
cube ([3, $l, $height]);
} 

difference() { 
//switch support
translate([66,$l/2+$wall+$slotsize+$outsidewall/2-6,$wall])     
cube ([4, 12.5, $height-3]);
 //slide switch
translate([$length-$outsidewall-3,25,$height/2])    
cube ([8,9.3,5.5]);   
}

//power plug
difference() { 
translate([66,$wall*1.5+6,$wall+5]) 
cube([4, 7.5, $height-8]);
translate([66.3,$wall*3+4.1,$wall+5] )  
cube([2.5, 5.2, $height-3]); 
rotate([90,0,0])
translate([67.4,6.5,-18])
cylinder(r=2,h=10); 
}
//lets curve this so it's easier to move the wires
difference () {
translate([65,$wall*2+1.2,$wall]) 
cube([4, 18, 5]);
rotate([90,0,90])
translate([14.3,9.5,66])
cylinder(r=10,h=10);  
}




