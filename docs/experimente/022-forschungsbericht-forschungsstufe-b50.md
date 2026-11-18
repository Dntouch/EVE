# Forschungsbericht – Forschungsstufe B.5.0

## A. Versuchsaufbau

B.5.0 reproduziert 2.688 Entdeckungs-, 1.152 Bestätigungs- und 1.024
Aufgabenläufe. Drei unabhängige Stromfamilien, normale und inverse
Zielzuordnung sowie 64 gepaarte Seeds bleiben erhalten. E1 und E2 werden aus
getrennten vollständigen Zustandskopien ohne Evaluationsplastizität ermittelt.

Die Kreditdiagnose wertet prospektiv 49.152 reale Trainingsereignisse aus. Je
Familie-Zuordnung-Seed wird genau das erste erfolgreiche Explorationsereignis
nach autonomer Stille als P1-Zustand kopiert. Eine zweite, getrennte P4-Probe
verlangt zusätzlich eine zuvor autonome andere Reizbeziehung. Mehrere
Ereignisse desselben Netzes werden nicht als unabhängige Stichproben gezählt.

## B. Referenz

384/384 B.4.9-K0-Reproduktionen stimmen in E1, E2, Gewichten,
Fingerabdrücken und Netzgültigkeit exakt überein. Die korrigierten K0-Werte
bleiben unverändert:

| Familie | Zuordnung | E1 | E2 | robuste E1-Seeds |
|---:|---|---:|---:|---:|
| 0 | normal | 43,68 % | 70,07 % | 5/64 |
| 0 | invers | 49,61 % | 73,78 % | 5/64 |
| 1 | normal | 46,84 % | 71,75 % | 1/64 |
| 1 | invers | 47,59 % | 71,05 % | 5/64 |
| 2 | normal | 41,98 % | 69,92 % | 1/64 |
| 2 | invers | 39,65 % | 66,98 % | 1/64 |

Keine der zwölf Entdeckungs- und Bestätigungszeilen erfüllt die allgemeinen
E1- und Robustheitskriterien.

## C. Prospektive positive Ereignisse

Alle 384 Seed-Cluster besitzen ein zulässiges erstes positives
Explorationsereignis. Es tritt zwischen Episode 0 und 8 auf. Der Reiz erzeugt
zunächst keine autonome Antwort; die allgemeine geschützte Exploration
aktiviert den richtigen Weg und die Umwelt bewertet die tatsächlich
entstandene eindeutige Handlung positiv.

Die Ziel-Reizsynapse erhält in 378/384 Fällen Eligibility und steigt von
Gewicht 0 auf 1. Die kausal aktiven Explorations- und Ausgangssynapsen sind
geschützt. In 99 Fällen wird zusätzlich eine konkurrierende Hemmsynapse
verstärkt. Die vorhandene Eligibility überbrückt somit Sensor- und
Explorationsphase grundsätzlich, verteilt Kredit aber teilweise auch an
nichtursächliche lokale Kandidaten.

## D. Warum entsteht nach einer Erfahrung kein Abruf?

Nach einer ersten positiven Erfahrung bleiben 384/384 P1-Kopien beim nächsten
Reiz ohne autonome Antwort. Der relevante sensorische Pfad besitzt Gewicht 1,
die Feuergrenze beträgt 2. Weder das Entfernen konkurrierender Änderungen noch
eine andere Modulationszeit innerhalb des gültigen Eligibility-Fensters
ändert diesen Befund.

Das Ergebnis bestätigt H3: Die richtige Synapse wird meist geändert, aber die
einmalige Änderung ist noch nicht wirksam genug. Eine positive Erfahrung ist
lokal gespeichert, jedoch noch keine autonom abrufbare Fähigkeit.

## E. Notwendige und hinreichende Änderung

| diagnostische Wiederholung | korrekter autonomer Abruf |
|---:|---:|
| nach 1 positiver Erfahrung | 0/384 |
| nach 2 positiven Erfahrungen | 364/384 |
| nach 3 positiven Erfahrungen | 384/384 |
| nach 4 positiven Erfahrungen | 384/384 |

Bei der zweiten Erfahrung entstehen 364 Änderungen des Zielpfads von 1 auf 2.
In getrennten Kopien ist jede dieser Änderungen notwendig: Rücknahme auf 1
beseitigt den Abruf. Sie ist zugleich hinreichend: Isolierte Übernahme von 2
stellt den richtigen Abruf her. Der clusterbewusste Anteil beträgt 94,79 %
mit einem 95-%-Intervall von 92,45 bis 96,88 %.

Dieser Versuch erzwingt keine positive Bewertung ohne erfolgreiche Handlung:
Jede Wiederholung enthält erneut Reiz, ausbleibende autonome Antwort,
geschützte Exploration, tatsächlich erfolgreiche Motoraktivität und erst
danach positive Modulation. Er ist dennoch eine Kausaldiagnose derselben
erfolgreichen Erfahrung und keine Aussage über ihre natürliche Häufigkeit im
gemischten Strom.

## F. Andere Beziehung und nichtursächliche Änderungen

Die P4-Stichprobe enthält 384 Ereignisse mit bereits korrekter anderer
Reizbeziehung. Nach der neuen positiven Erfahrung sind 22/384 neue
Beziehungen korrekt; die bestehende Beziehung bleibt in 376/384 Fällen
erhalten und beide sind in 22/384 Fällen gleichzeitig korrekt. Nur
sensorische Gewichtsänderungen erzeugen 40/384 neue richtige Abrufe. Die
zusätzlichen lokalen Hemmungs- und Konkurrenzänderungen sind daher nicht
notwendig und können einzelne neue Bindungen unterdrücken.

Die unmittelbare Zerstörung der bestehenden Beziehung ist mit 8/384 Fällen
klein. H4 erklärt den ersten Erwerbsfehler nicht. Langfristig ist die Wirkung
dennoch ungünstig: Nach 16 identischen zukünftigen Erfahrungen liegt die
positive D0-Kopie im Mittel 3,63 Punkte unter der Kontrolle ohne positive
Gewichtsänderung. H5 und spätere Interferenz bleiben daher relevant.

## G. Kontrollgruppen und neue Funktion

K0–K3 laufen als vollständige Referenzmatrix. K4 bis K8 sind ausdrücklich
diagnostische Zustandskopien: keine positive Plastizität, unveränderte
positive Eligibility, isolierte Sensoränderungen, isolierte konkurrierende
Änderungen und verzögerte Modulation. Sie sind keine neuronalen Lernregeln.

K9 und K10 werden nicht implementiert. Die bestehende lokale Zuordnung ist
unter wiederholter erfolgreicher Erfahrung grundsätzlich ausreichend. Ein
neuer Kandidatenmechanismus ist weder notwendig noch durch die erste
subthreshold Änderung begründbar.

## H. Antworten A bis J

1. **A – erreichte Spuren:** Ziel-Reiz-, geschützte Explorations- und
   Ausgangsspuren sowie teilweise konkurrierende Hemmspuren erhalten lokale
   Eligibility.
2. **B – kausale Verbindung:** Der zweite Schritt der Ziel-Reizsynapse von 1
   auf 2 ist in 364/364 passenden Kopien notwendig und hinreichend.
3. **C – nichtursächliche Kandidaten:** Ja. 99 Hemmänderungen tragen nicht zur
   ersten erfolgreichen Motoraktivität bei und verbessern den Abruf nicht.
4. **D – Sensor-Explorations-Verbindung:** Ja, grundsätzlich. Die sensorische
   Nullgewicht-Synapse wird lokal aufgenommen und verstärkt.
5. **E – Wirksamkeit:** Ja. Nach einer Erfahrung bleibt Gewicht 1 unter der
   Feuergrenze 2.
6. **F – andere Beziehungen:** Unmittelbarer Verlust ist selten; zusätzliche
   gemeinsame lokale Änderungen und spätere gemischte Erfahrung können die
   Gesamtleistung dennoch verschlechtern.
7. **G – bestehende Mechanismen:** Ja, im isolierten Wiederholungsversuch
   reichen sie nach zwei bis drei real erfolgreichen Erfahrungen aus.
8. **H – neue Kreditzuordnung:** Nein. Ein fehlender Zuordnungsmechanismus ist
   nicht kausal belegt.
9. **I – Stabilität:** Isolierter Abruf nach Wiederholung ist belegt, stabile
   Erhaltung im gemischten Erfahrungsstrom nicht.
10. **J – Bedeutung:** Mehrfachlernen benötigt nicht nur korrekten lokalen
    Kredit, sondern ausreichend wiederkehrende positive Erfahrung und Schutz
    vor späterer konkurrierender Veränderung.

## I. Abschluss

B.5.0 trennt Kreditzuordnung von synaptischer Wirksamkeit. Die vorhandene
lokale Ereignisgeschichte verbindet einen zunächst wirkungslosen Sensorpfad
mit der explorativ erfolgreichen Entscheidung. Eine einzelne positive
Modulation erzeugt Gewicht 1 und bleibt unter der Schwelle. Eine zweite
Änderung auf Gewicht 2 ist im isolierten Zustand kausal notwendig und
hinreichend.

Das negative Gesamtergebnis bleibt: Die natürlichen gemischten Ströme führen
nicht zuverlässig zu wiederholten, selektiven und stabilen positiven
Bindungen. Eine neue Kreditregel wird nicht freigegeben. B.5.1 sollte deshalb
die natürliche Wiederholung erfolgreicher Erfahrungen, die Schwellenpassage
und den anschließenden Stabilitätsverlust untersuchen, ohne Lernschritt oder
Feuergrenze vorschnell zu verändern.

