# Technische Analyse – lokale Membran-Recovery B.5.4

## 1. Audit der historischen Referenz

Der Konfigurationspfad B.4.6 bis B.5.3 verwendet tatsächlich
`ResetMode::to_zero`. Der Wert wird nicht im Forschungsrunner überschrieben,
sondern aus dem Standard der `ExperimentConfig` übernommen. Die frühere
Beschreibung als `subtract_threshold` war unzutreffend und wurde korrigiert.
Die historischen Daten und Versuche wurden nicht verändert.

Diese Korrektur ändert die Schlussfolgerung aus B.5.3 nicht: Dort wurden reale
Zustände vor und nach Verlusten kopiert. Die kausale Wirkung der beobachteten
Membranen bleibt unabhängig von der zuvor falsch benannten Resetsemantik
bestehen.

Die historische Referenz besitzt außerdem:

- `refractory_input_events = 0`,
- `RecoveryMode::input_dependent`,
- `recovery_delay = 16`,
- `membrane_relaxation_percent = 0`.

## 2. Ereigniskette

Ein Recoveryereignis wird nur nach einem Feuerereignis und nur in R1 oder R2
geplant. Dabei wird die lokale Recoverygeneration erhöht und ein Ereignis mit
der konfigurierten Verzögerung eingereiht. Subthreshold-Zustände planen kein
Recoveryereignis.

`recover_neuron()` führt ein Ereignis nur aus, wenn der Modus nicht R0 ist,
die Generation noch stimmt und `refractory_events_remaining > 0` gilt. Bei
Refraktärwert 0 werden R1/R2-Ereignisse daher zwar geplant, später aber als
veraltet verworfen. Bei erneutem Feuern macht die neue Generation ältere
Ereignisse ungültig. Bei gleichem Zeitstempel wird das beim Feuer geplante
Recoveryereignis vor dem nachfolgenden Eligibility-Ablauf verarbeitet; die
Reihenfolge ist reproduzierbar.

R1 beendet bei einer gültigen Recovery nur den Refraktärzustand. R2 wendet
zusätzlich die konfigurierte Membranrelaxation auf den dann aktuellen lokalen
Membranwert an. R2 mit 0 % ist in allen gepaarten Naturverläufen exakt gleich
R1.

## 3. Deterministische Mikroversuche

Die vollständige technische Matrix umfasst Refraktärwerte 0, 1 und 2,
Verzögerungen 1, 4, 8, 16, 32 und 64 sowie Relaxationen 0, 10, 25, 50, 75 und
100 %. Die Beobachtungen bestätigen:

- R0 plant kein zeitgesteuertes Recoveryereignis.
- R1/R2 mit Refraktärwert 0 erzeugen ausschließlich veraltete Recoveries.
- R1/R2 werden mit Refraktärwert 1 oder 2 tatsächlich aktiv.
- R1 erhält den aktuellen Membranwert.
- R2 relaxiert ihn lokal; 100 % setzt den bei Recovery vorhandenen Wert auf
  null.
- Veraltete Generationen erzeugen keine Mehrfachrelaxation.
- Erregende und hemmende Integration sowie Netzvalidität bleiben erhalten.
- Ein nicht feuerndes Neuron behält sein Subthreshold-Potenzial und plant
  keine Recovery.

Die letzten beiden Punkte markieren die Reichweitengrenze: Die vorhandene
Recovery kann nur Zustände verändern, nachdem dasselbe Neuron bereits die
Feuerschwelle überschritten hat.

## 4. Zeitliche Integration

Für AA, BB, AB, BA, ABA und BAB wurden Abstände 0, 1, 2, 4, 8, 16, 32 und 64
getestet. Jeder Einzelimpuls war unterschwellig, zwei Impulse zusammen
erreichten die Schwelle. Alle 48 Folgen je technischer Variante behielten
diese Integration bei. Das ist mechanistisch folgerichtig: Vor dem ersten
Feuern existiert noch kein Recoveryereignis, das die positive Voraktivität
abbauen könnte.

Der Befund ist kein allgemeiner Nachweis, dass beliebige Recoveryparameter
zeitliche Information erhalten. Er zeigt enger, dass die bestehende
feuergebundene Implementierung die notwendige Prä-Feuer-Integration in dieser
Kontrolle gar nicht erreichen kann.

## 5. Persistenz und Ressourcen

Alle 256 Persistenzfälle für R0, R1, R2-0 und R2-50 wurden vor einer fälligen
Recovery gespeichert und geladen. Fingerabdruck, Fälligkeitsverarbeitung,
Fortsetzung und Netzvalidität stimmen jeweils exakt überein.

R1/R2 verarbeiten in den natürlichen Läufen rund 2,00 zusätzliche
Recoveryereignisse je Episode. Die maximale Queue wächst von 24 in R0 auf 28;
der geschätzte Runtime-Speicher bleibt bei 24.512 Byte. Es entstehen keine
unkontrollierten Schleifen und in der gewählten wirksamen Matrix keine
veralteten Recoveries.

## 6. Technische Schlussfolgerung

Die vorhandene Architektur ist deterministisch, lokal und persistent, aber
ihre Wirksamkeit hängt zwingend von einem aktiven Refraktärzustand ab. Sie
erreicht ausschließlich Membranzustände bereits feuernder Neuronen. Gerade
unterschwellige Restpotenziale, die eine spätere Entscheidung beeinflussen
können, bleiben ohne weiteres Feuer zeitlich unbegrenzt erhalten. Dieser
konkrete Reichweitenmangel ist die technische Ausgangsfrage für B.5.5.

