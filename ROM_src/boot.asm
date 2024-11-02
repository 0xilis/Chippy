# This is a very poorly coded testROM for Chippy, but ig it works... - Snoolie K

:const true 1
:const false 0
:const false_evilhack 0x10

# any_key_press
#
# Detects any key pressed. Intensive on CHIP-8.
# Originally, this was going to indicate by calling return_true/return_false
# However, it is slightly better for speed if we just return and check
# For false by checking if v0 is not 0x10 / 16.
: any_key_press # Slow AF, it's noticable how much this slows down the logo ):
  if v0 == false_evilhack then jump any_key_press_ignore # Only work 1/2 frames for speed
  v0 := 0
  loop
    if v0 key then return # If we wanted any_key_press to ignore 0 we could change jump return_true to return
    v0 += 1
    if v0 > 0xF then return # Returns false_evilhack to save 2 instructions
  again
  
: any_key_press_ignore
  v0 += 1
  return
  
: return_true
  v0 := true
  return
  
: return_false
  v0 := false
  return
  
: main
  v1 := 0
  v2 := 12
  i  := CH_img
  sprite v1 v2 8
  v3 := 9
  i  := IP_img
  # sprite v3 v2 8
  v4 := 18
  i  := PY_img
  sprite v4 v2 8
  v0 := false_evilhack
  loop
    clear
    v1 += 1
    v3 += 1
    v4 += 1
    i := CH_img
    sprite v1 v2 8
    i := IP_img
    sprite v3 v2 8
    i := PY_img
    sprite v4 v2 8
    
    any_key_press
    if v0 <= 0xF then jump menu # Originally this was != false_evilhack but wanted to 1/2 frame
    
    # lock the framerate of this program via the delay timer:
    loop
      vf := delay
      if vf != 0 then
    again
    vf := 3
    delay := vf
  again
  
: menu
  clear
  loop
    v0 := key
    if v0 == 0x4 then jump main
  again
  
: __PANIC #debug panic/fail screen
  clear
  v1 := 0
  v2 := 0
  i := IP_img
  sprite v1 v2 8
  loop
  again

: CH_img
  0xF9 0x89 0x8F 0x89 0xF9
# *****  *
# *   *  *
# *   ****
# *   *  *
# *****  *

: __PAGEZERO1 #hack around not being able to use 5x8 sprites so it will reach here when going OOB
  0x00 0x00 0x00 0x00 0x00
: IP_img
  0xEF 0x49 0x4F 0x48 0xE8
# *** ****
#  *  *  *
#  *  ****
#  *  *
# *** *
: __PAGEZERO2
  0x00 0x00 0x00 0x00 0x00
: PY_img
  0xF5 0x95 0xF2 0x82 0x82
# **** * *
# *  * * *
# ****  *
# *     *
# *     *

: __PAGEZERO
  0x00 0x00 0x00 0x00 0x00