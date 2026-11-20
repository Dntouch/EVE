# Forschungsbericht – Forschungsstufe B.5.9

## Ergebnis

B.5.9 erfasst erstmals die tatsächliche lokale Entscheidungsmarge zwischen
synaptischem Eingang und Feuerentscheidung und verbindet sie mit Eligibility
und späterer Plastizität. Die zusätzliche lokale Information ist real und
verbessert die Prognose, reicht aber nicht für eine neue Lernregel.

Die unabhängige Vorhersage steigt von 49,49 % mit der B.5.8-Signatur auf
53,51 % mit zeitlicher Entscheidungskonkurrenz. Dennoch bleiben 90,50 % der
Beobachtungen selbst auf L4 in lokal identischen Signaturen mit verschiedenen
globalen Folgen. Eine lokale Feuerentscheidung ist somit weder ein globales
Erfolgslabel noch ein Beweis, dass eine spätere Gewichtsänderung nützlich ist.

## Zentrale Befunde

### Exakte Reproduktion

B.5.7 wurde mit 242/384 und 254/384 robusten Netzen exakt reproduziert;
B.5.8/B.5.9 Fest-16 mit 240/384 und 244/384. Alle 4.224 zentralen Netze waren
gültig. Die Ergebnisse für Fest-1, Fest-12, Fest-32, unregelmäßige Abstände,
R2, S5 und No-Learning entsprechen den präregistrierten Kontrollen.

### Entscheidungsmarge und Nichtfeuern

Von 27.913 lokalen Integrationen feuerten 12.288 bei einer Marge größer oder
gleich null. 15.625 Ereignisse feuerten nicht. Negative Marge erklärt lokale
Unterschwelligkeit; der gesonderte Refraktärzustand erklärt 985 unterdrückte
Entscheidungen. Nichtfeuern erzeugte in diesem Korpus keine Eligibility.

Die Marge ist damit eine präzise Beschreibung des neuronalen Entscheids, aber
keine Bewertung seiner späteren funktionalen Wirkung.

### Kausale Eingangsbeiträge

Die Entfernung eines einzelnen erregenden Beitrags änderte in 15/96 Kopien
Feuerung und Handlungskorrektheit. Die untersuchten inhibitorischen Beiträge
änderten 0/96 Fälle, weil sie ausnahmslos auf bereits auf null begrenzte
Motor-Membranen trafen. Vorhandensein, Gewicht und zeitliche Eligibility
beweisen folglich keine kausale Wirksamkeit.

### Lokale Information

L1 bis L3 verbessern die unabhängige Prognose schrittweise. L4 ergänzt den
relativen Kandidatenanteil, trennt gegenüber L3 aber weder weitere Signaturen
noch höhere Genauigkeit. Der L4-L0-Gewinn von 4,23 Prozentpunkten ist
statistisch bestätigt, aber wissenschaftlich nicht mit Regelkausalität
gleichzusetzen.

### Positive Konkurrenz

Die zwölf historischen Fälle sind nicht länger nur am späten Endzustand
beobachtet. Ab dem ersten gemeinsamen Erwerb gingen alle zwölf Beziehungen
nach positiven und ohne negative Änderungen verloren. Von 52 erhöhten
Synapsen stellten zehn bei isolierter Rücknahme beide Beziehungen wieder her;
die gemeinsame Rücknahme aller zwischenzeitlichen Verstärkungen tat dies in
12/12 Fällen. Positive Verstärkung ist damit in konkreten Fällen kausal
schädlich, bleibt in anderen Fällen jedoch für Erwerb und Umlernen notwendig.

### Fähigkeitserhalt und Umlernen

Der B.5.8-Befund bleibt bestehen: 90/96 schreibgesperrte gegenüber 62/96
aktiven Kopien blieben robust. Nach Umweltumkehr erreichten dauerhaft aktive,
weiter gesperrte und reaktivierte Kopien 60/96, 0/96 und 61/96. Eine pauschale
Sperre löst den Selektionskonflikt nicht.

## Antworten A–S

**A. Welche Eingänge bestimmen die Feuerentscheidung?** Tatsächlich
angewandte erregende Beiträge erhöhen, hemmende Beiträge senken die lokale
Membran; Refraktärzustand und Clamping begrenzen die Wirkung. K6 bestätigt
kausale Erregungsbeiträge, während die beobachtete K7-Hemmung am Null-Clamp
wirkungslos blieb.

**B. Welche Bedeutung besitzt die Entscheidungsmarge?** Sie beschreibt exakt
den Abstand zur Feuergrenze vor Reset. Sie erklärt Feuerung und
Unterschwelligkeit, aber nicht den globalen Nutzen.

**C. Wie unterscheiden sich Beiträge?** Notwendige Beiträge verändern bei
Ablation Feuerung oder Verhalten; redundante Beiträge wirken erst kombiniert;
nicht nachweislich wirksame Beiträge ändern die festgelegte Probe nicht. Ein
Gewicht allein unterscheidet diese Klassen nicht.

**D. Was fehlt bei Nichtfeuern?** Lokal fehlen ausreichende wirksame Erregung
oder Verfügbarkeit außerhalb der Refraktärphase. Für eine globale Erklärung
fehlen Zielbedeutung und Gegenbeziehungszustand.

**E. Sind fehlende Motorantworten lokal erklärbar?** Ja, als fünf konkrete
Integrationsklassen. Welche davon global korrigiert werden sollte, bleibt
jedoch mehrdeutig.

**F. Wie hängt Feuern mit Eligibility zusammen?** In diesem Korpus erzeugten
alle 12.288 Feuerentscheidungen Eligibility; 15.625 Nichtfeuerentscheidungen
nicht. Eligibility markiert Beteiligung, nicht Nützlichkeit.

**G. Trennt die Marge notwendige von schädlichen Änderungen?** Sie verbessert
die Prognose als Teil von L2, trennt die Klassen aber nicht sicher.

**H. Was liefert erregende/hemmende Konkurrenz?** L1 erhöht Prognose und
erklärt Nettointegration. Die inhibitorische Kausalprobe zeigt zugleich, dass
ein registrierter Beitrag durch Clamping funktional null sein kann.

**I. Was liefert Reihenfolge?** L3 verbessert die Signatur geringfügig.
Gleichzeitigkeit, Abstand und Bündelposition beseitigen die Mehrdeutigkeit
nicht.

**J. Wie unterscheiden sich L0–L4?** Die unabhängige Genauigkeit beträgt
49,49 %, 51,19 %, 53,46 %, 53,51 % und 53,51 %. L4 fügt gegenüber L3 keinen
weiteren beobachtbaren Trenngewinn hinzu.

**K. Welche Mehrdeutigkeit bleibt?** 90,50 % der L4-Beobachtungen gehören zu
Signaturen mit mehreren späteren Folgen. Lokale Dynamik enthält kein sicheres
globales Erfolgsurteil.

**L. Welche positiven Änderungen verursachen Verluste?** In den zwölf Fällen
waren 10/52 einzelne Erhöhungen hinreichend für die Wiederherstellung bei
Rollback; die kombinierte Rücknahme wirkte in 12/12. Acht dieser zehn
Einzelursachen waren erregend, zwei inhibitorisch.

**M. Welche negativen Änderungen sind erforderlich?** Die B.5.8-Kontrollen
bestätigen ihre Bedeutung für Korrektur und Umkehr. In den zwölf erstmals vom
Erwerb verfolgten Verlusten trat dagegen keine negative Änderung auf.

**N. Bleiben Fähigkeiten ohne Dauerschreiben erhalten?** Ja, 90/96 gegenüber
62/96 robusten Fällen über 512 Erfahrungen; für Umweltumkehr muss Lernen wieder
aktiv sein.

**O. Welche lokale Information könnte Nichtändern erlauben?** Marge,
Beitragsrichtung, tatsächliche Wirksamkeit und zeitliche Beteiligung sind
Kandidaten. Keine beobachtete Kombination erreicht die nötige Eindeutigkeit.

**P. Ist die Unterscheidung bereits möglich?** Nicht mit L0–L4. Eine
allgemeine Unmöglichkeit künftiger lokaler Mechanismen ist nicht bewiesen.

**Q. Was erklären die zwölf Fälle?** Sie werden von unspezifischen späten
Endzuständen auf konkrete positive Konkurrenz seit dem ersten gemeinsamen
Erwerb und zehn einzeln kausale Verstärkungen eingegrenzt.

**R. Ist eine neue Regel gerechtfertigt?** Nein. Der statistische Gewinn ist
klein gegenüber der verbleibenden Mehrdeutigkeit, und eine Rollback-Regel wäre
eine unzulässige technische Sicherung.

**S. Was folgt für B.5.10?** Die genaue zeitliche Kausalkette der zehn
schädlichen Verstärkungen soll prospektiv am Schreibzeitpunkt untersucht
werden: Welche wiederholt lokal verfügbare Beitragsgeschichte unterscheidet
sie von notwendigen positiven Änderungen, ohne Zielwissen oder Rollback?

## Grenzen

- Die Detailstichprobe umfasst 96 präregistrierte Ausgangszustände.
- L4 beweist nur die Grenze des untersuchten Informationsinventars.
- Die historische Rücknahme ist eine Diagnose nach Kenntnis des Verlusts und
  keine online verfügbare neuronale Regel.
- Ein unmittelbar neutraler Effekt kann später nützlich oder schädlich werden.
- Die inhibitorische Nullwirkung gilt für die beobachteten Zustände am
  Null-Clamp, nicht für Hemmung allgemein.

## Merge-Empfehlung

**B.5.9 nach `main` mergen: Ja.**

696/696 Regressionstests bestehen. Reproduktion, unabhängige Bestätigung,
Persistenz und deterministische Fortsetzung sind erfolgreich; die Beobachter
sind verhaltensneutral. Es wird keine neue Lernregel aktiviert, die
Schreibsperre bleibt Diagnostik und S4 bleibt standardmäßig aus.
