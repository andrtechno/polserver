## Изменения относительно Classic-dist


#### Изменение скиллов:
* **Lumberjacking**
    * добавлена добычна видов дерева
* **Mining**
    * изменена добыча видов дерева (добавлены новые виды)
    * добавлена функция одевание кирки.
* **Blacksmith**
    * добавлена функция одевание молотка.
* **Hiding**
    * Изменены тексты на unicode "RUS"
* **Alchemy**
    * Добавлены новые виды дутылок(homeric,invisibility etc.)
* **Carpentry**
    * добавлена анимация изготовки предметов.
* **ДОБАВЛЕН СКИЛЛ Stooping & Stealing.**
* **Detect hidden**
    * изменен радиус под скилл 150.
    * убрана активация скила по таргиту.
    * изменены тексты на unicode "RUS"
* **Hiding**
    * изменены тексты на unicode "RUS"
* **Stealth**
    * Разрешение ходить в стелсе на коне: (stealthhorse - npcdesc.cfg)
* **Begging**
    * изменены тексты на unicode "RUS"
* **Cooking**
    * Изменена логика прикотовки рыбы
* **Tailoring**
    * изменен объект шерсти на 0x0DF8 (стандартный)

#### PKG:
- Добавлен **items/dangesgate** - портыла в данжы
- Добавлен **items/plus** - Плюс вещи на скилы
- Изменено **items/runebook** - Исправлена руна бука (recall & gate).
- Добавлен **items/christmas** - Новогодний пакет
- Добавлен **system/charclass** - Классы персонажей (в разработке)
- Добавлен **items/staffrobe** - Стафф роба (при одевании меняет цвет робы see: cmds.cfg, при клики на робу concealed)
- Добавлен **items/nodamageweapon** - в разработке
- Добавлен **items/shirinkpotion**


#### Commands (textcmd)
##### player
- **.where** - показывает текущие место положение X Z Y
##### test
- **.count [int]** - показывает количество вешей в радиусе [int]
- **.setcolor [color]** - устанавливает цвет вещи
- **.unload [scriptname]** - перезагрузка скрипта
- **.mdestroy [int]** - удаление вещей в радиусе [int]
- **.necrobook [int]** - добавлена создание болной некробук [int]
#### Изменение include's/scripts
* **vetement.inc**
    * доюавлена возможность делать вещь newbie
* **death.src** добавлена функция удаление destroy_node_merc если это продавец



Изменена система кастов(unmod.src,statmod.inc) и анорт магия

+добавлены баффы/дебафы
bless,cunning,agility,protection,strength,
incognito,polymorph,magic reflection,paralyze,reactive armor,Invisibility
clumsy, weaken ,feeblemind
poison,cure => system/poisonwatcher/poison.inc
light - проверить на удаление.
-  сделать еще нужно

necro (еще требуется)

1043 curse


 - проверить
mass cure
1044 mass curse
mass dispel


DisplayDamage() fix double display damage if self damage


