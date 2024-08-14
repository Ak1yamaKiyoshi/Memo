
spaced repetition of memories:

memory#1
memory#2
memory#3 
memory#4 

seen#1 -> +2 days -> seen#2 -> +3 days -> seen#3
day 0, day 2, day 5

so: 
#1 
    penalty;
    first seen;
    todays date;
    days;
    for each time seen: 
    days += diff (time seen cur, time seen prev)

    if todays date > first seen + days + penalty:
    need to repeat


#2
one of that:
  remove 1/3 of words
  insert random characters across 
  reverse string 

go trough each memory:
  get todays memories #1
  show to the user transformed #2
  ask if user needs to repeat it (y, n, m)
  add penalty in days to memory. 

memories should have then:
  penalty_days
  repetition days that contain days when user repeated the memory 
  to repeat flag, we don't want to repeat every single one of memmory

additional:
  display flag

nice to have features: 
  pipe `n` search results to certain operation: like add tag, remove tag, link to memory

also, as part of gamification:
    make a market, where you can buy functions from currency
    currency earned by adding and editing memories, 
    more complex operations should give player more currenc
```











---

/*
спогад ; коли     ; згадав чи нє ; днів пропущено
int    ; timstamp ; bool         ; int 
1      ; вчора    ; y            ; 2 
1      ; -3 дні   ; n            ; 2 
*/

n = базова кількість днів проміжку
k = сума "згадав чи нє" для кожного спогаду 

кількість днів від початку, через яку треба показати спогад = кількість записів ("коли") * n + k - "дні пропущено"


memory:
  text : str 

/
memories:
  memory01
  memory02 
  memory03
  memory04
  memory05
  memory06
  memory07
  memory08


/r - /repetition

/r
  йдемо по кожному спогаду -> визначаємо сьогоднішні -> додаємо penalty для пропущених 
  /* Вирізати слова зі спогаду -> вирізати лише дієслово / іменник (якщо закінчення відповідає іменнику або дієслову, додаємо у стек для вирізання, та вирізаємо якийст % з нього,  та формуємо спогад у наступному вигляді : я [    ] щось там ). -> покращити маленькою nlp моделькою. 75% 
   * Перемішувати слова + реверс стрінги
   * Вставляти випадкові символи у випадкові місця тексту */
   + додати реролл для всього цього ^^^

  спитати юзера чи він згадав? 
  /* Якщо згадав - нагадати через n+k днів -> last seen.append(timestamp) зараховується  
   * Якщо не згадав - нагадати сьогодні або наступного разу -> не зараховується; penalty += n + k */

penalty = кількість пропущених днів + ()

---
Ігровий режим (можна включати та виключати), базується на економіці
  отримуєш валюту за додавання спогадів, за їх редагування та видалення. 
   - за валюту можна придбати:
      пошуки та повтори. -> купити кращий пошук 
        (tf-idf -> levenshtain distance symbolwise -> keywords -> levenstain wordwise + keywords-> jaccard similarity)
        (пошук по тегах)
        (пошук по датах)

---

> знаходиш спогади
> поставити n`ній кількості з них тег | видалити | приховати 

memory1 -> memory2
memory1 -> memory3
memory2 -> memory4

memory3: memory1: memory4 | memory2
якщо jaccard similarity < threshold то показувати результати з прив'язаних спогадів.

Ім'я 
подія0 про Ім'я -> Ім'я
подія1 про Ім'я -> Ім'я
подія2 про Ім'я -> Ім'я
подія3 про Ім'я -> Ім'я

--- 



memory_id : memory_id

memory:
  linked_memories: ref



```
```
Memory#1
Memory#2 #todo
Memory#3
мене вкусила бджола #4 <- побачиш 
Memory#5
я з'їв кота #6 <- не треба показувати 
Memory#7
Memory#8 #todo

/repetition 
-> Memory#2 -> Memory#5

фільтр по тегах, 
відмічать чи спогад треба показувати тут 

/repetition #todo 
Memory#8 


---
Anki -> 
заносиш туди картки з словами, описуєш їх якось, або даєш з іншого боку картки переклад
показу є раз на кілька днів шоб ти просто запам'ятав 

```
