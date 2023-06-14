======================================
Combat Book Package
By Rean Opperman (Aeros)
Legends of Chyrellos UO Shard
http://www.chyrellos.co.za

Special thanks to:
MuadDib - for identifying the packets
======================================

This package hooks the combat book (the purple hook in the paperdoll), enabling you to set the primary/secondary weapon ability on a character.

This package is very simple to install. Simply drop it into your packages folder. However, you will have to change your hitscripts to take the ability into consideration, and also so that the ability gets disabled again upon use.

To disable the ability after use, call the RemoveAbility(who) function, which can be found in combatAbility.inc in the package.
