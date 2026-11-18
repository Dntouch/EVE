# Entscheidung 0017: Motorische Konkurrenz ist der nächste lokale Engpass

- Status: angenommen
- Datum: 18. November 2026
- Bezug: Entscheidungen 0015–0016

## Kontext

B.4.5 reproduziert die erfolgreichen Sequenzwerte von B.4.4, aber nicht
gemischtes Lernen. Zeit-, Dauer-, Feedback- und Kandidatenkapazitätsvariation
verbessern K1 nicht. In der Referenz erreichen keine Modulationen ältere
Episodenspuren, während doppelte Motoraktivierungen im Training zunehmen.

Negative Modulation ausschließlich bei Doppelaktivierung verbessert die
Mischleistung kausal um 40,97 Punkte gegenüber K1 und reduziert Doppelaktionen
bei allen Seeds. Sie verfehlt trotzdem die Einzelschwellen beziehungsweise das
Robustheitsziel in Entdeckungs- und Bestätigungsdaten.

## Entscheidung

H2 – lokale motorische Entscheidungskonkurrenz – wird als dominanter nächster
Engpass angenommen. H1 wird für diesen Aufbau nicht als dominante Ursache
angenommen. Die vorhandene `active_phase`-Bindung bleibt nützlich für
Sequenzen, löst aber H2 nicht.

Pauschale negative Rückmeldung bei Doppelaktivierung wird nicht als allgemeine
Lernregel freigegeben. C1 wird in B.4.5 nicht eingeführt. C2 wird für B.4.6 nur
als Forschungsauftrag vorbereitet.

## Anforderungen an B.4.6

Eine lokale Konkurrenzvariante muss:

1. ausschließlich durch neuronale Ereignisse ausgelöst werden;
2. ohne technische Wahl von A oder B und ohne Queue-Reihenfolgebias arbeiten;
3. allgemeine lokale inhibitorische Semantik statt fest verdrahteter
   Handlungslogik verwenden;
4. Nullgewicht-Lernwege und geschützte Exploration erhalten;
5. vollständige Persistenz, Abschaltkontrolle und Zustandskopie-Ablation
   besitzen;
6. K0/K1/K2 sowie Einzel-, Sequenz-, Alternierungs- und Mischregressionen
   wiederholen;
7. mindestens 48/64 robuste gemischte Seeds in Entdeckung und Bestätigung
   erreichen, bevor eine Freigabe erwogen wird.

Die allgemeine Freigabesperre bleibt bestehen. B.4.5 weist keine robuste
selbst erworbene gemischte Fähigkeit nach.
