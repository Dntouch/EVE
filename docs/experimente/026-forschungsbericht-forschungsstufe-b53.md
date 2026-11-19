# Forschungsbericht – Forschungsstufe B.5.3

## A. Ergebnis in einem Satz

Die transienten Abrufstörungen werden nahezu vollständig durch die lokale
Membranlage der Entscheidungsneuronen verursacht; G3, Eligibility und
Aktivitätszähler beeinflussen den unmittelbaren autonomen Abruf nicht, doch
eine frühere Membran technisch wiederherzustellen ist noch keine zulässige
allgemeine Gedächtnisfunktion.

## B. Reproduktion und Gültigkeit

B.5.2 wird getrennt und bis auf maschinenabhängige Performancewerte bytegenau
reproduziert. B.5.3 verwendet unverändert 768 Netze, 393.216 Episoden, drei
Stromfamilien, normale und inverse Rollen sowie unabhängige Entdeckungs- und
Bestätigungs-Seeds. Alle 15.194 Detailfälle bestehen die A/B-, Kopie- und
Gesamttransienten-Kontrollen. Die Auswahlgruppen werden getrennt berichtet.

## C. Kausale Zerlegung

Alte Membranpotenziale stellen 14.731/15.194 Detailantworten wieder her; neue
Membranpotenziale lösen in 14.636/15.194 früher korrekten Zuständen den Verlust
aus. Beide Werte reproduzieren exakt die entsprechenden B.5.2-Gesamtbefunde.

Bei allen 12.452 Verlusten ohne Gewichtsänderung ist T1 notwendig und
hinreichend. T2 Eingangsaktivität, T3 G3, T4 Eligibility, T5
Synapsenaktivität, T6 Recovery, Kontext, Zufallszustände und Diagnosemetriken
ändern den unmittelbaren E1-Abruf in keinem Fall. Kombinationen ergänzen den
Membraneffekt nicht.

Von 16.427 geänderten Einzelmembranen liegen 16.326 auf
Entscheidungsneuronen. In 14.728 Fällen genügt die Rücknahme einer einzelnen
Entscheidungsmembran; nur drei Fälle benötigen zwei geänderte Membranen
gemeinsam. Die Ursache ist damit lokal stark eingegrenzt.

## D. Probe- und Zeitwirkung

Frische Kopien bleiben deterministisch. Auf einer fortgesetzten Runtime
verändert bereits ein Probeimpuls die Membranlage und damit die nächste
Antwort. Der Effekt hängt vom zuerst präsentierten Reiz ab. Dagegen ergeben
ereignisfreie Abstände von 0 bis 64 Zeiteinheiten identische Werte, weil die
Referenzkonfiguration ohne fälliges Recoveryereignis keine passive
Membranrelaxation besitzt.

G3, Eligibility, Eingangs- und Synapsenaktivität ändern sich während der
Probe, sind jedoch durch Einzel- und Kombinationsinterventionen als Ursache
des unmittelbaren zweiten Abrufs ausgeschlossen.

## E. Zwei Beziehungen und Gewichte

Mit neuen Gewichten und alter Membran wird der verlorene Pfad in 14.731
Fällen korrekt, beide Pfade aber nur in 3.209 Fällen. Mit vollständig alten
Gewichten wird der betrachtete Pfad in 15.194 Fällen korrekt, die andere
Beziehung lediglich in 3.397 Fällen. Die Zustände stammen aus realen
Verlustzeitpunkten, an denen die andere Beziehung häufig bereits nicht
verfügbar ist.

Die Membranintervention beweist deshalb lokale Abrufkausalität, aber keine
robuste gemeinsame Stabilisierung. Sie kennt außerdem technisch den früheren
Zustand und ist keine neuronale Regel.

## F. Hypothesen

- H1 ist stark bestätigt: Membranpotenziale bestimmen den Detailverlust.
- H2 ist für unmittelbaren E1 widerlegt: G3 wirkt über Eligibility, nicht
  direkt auf die Feuerentscheidung.
- H3 ist für unmittelbaren E1 widerlegt: Eingangs- und Synapsenzähler wirken
  auf Phase und Plastizität, nicht unmittelbar auf die Ausgabe.
- H4 ist für unmittelbaren E1 widerlegt und für spätere Modulation offen:
  Eligibility allein ändert keine autonome Probe.
- H5 ist bestätigt: Reihenfolge verändert die lokale Membranlage.
- H6 ist auf drei Fälle begrenzt: Fast immer genügt eine Einzelmembran.
- H7 ist bestätigt: Der Probeimpuls ist selbst eine lokale
  Zustandsintervention.

## G. Antworten A bis L

1. **A:** Membranpotenziale und grundsätzlich Refraktär-/fällige
   Ereigniszustände können den Abruf direkt beeinflussen. In der Stichprobe
   ist ausschließlich die Membrankomponente kausal aktiv.
2. **B:** Metriken sind reine Diagnosezähler. G3, Eligibility,
   Eingangsaktivität und Synapsenaktivität sind keine bloßen Zähler, wirken
   aber erst auf Kandidatenbildung oder spätere Plastizität.
3. **C:** Die neue Membranlage, fast immer die eines Entscheidungsneurons,
   verhindert die zuvor korrekte Antwort.
4. **D:** Ja. Die alte Membrankomponente stellt 14.731 Antworten wieder her.
5. **E:** Nur in drei wiederherstellbaren Fällen sind zwei geänderte
   Membranen gemeinsam erforderlich. Andere Zustandsklassen ergänzen T1 nicht.
6. **F:** Membranpotenziale wirken direkt an der Feuergrenze. G3 liefert
   Kandidaten für Eligibility und wirkt nicht unmittelbar auf E1.
7. **G:** Eligibility beeinflusst ohne Modulation keinen unmittelbaren Abruf,
   sondern ausschließlich spätere Lernänderungen und ihre Ablaufsteuerung.
8. **H:** Der Probeimpuls integriert erregende und hemmende Beiträge in lokale
   Membranen; Restpotenziale verändern den nächsten Schwellenvergleich.
9. **I:** Abstände bis 64 haben in R0 keinen Effekt, solange kein fälliges
   lokales Ereignis die Membran verändert.
10. **J:** Einzelne verlorene Beziehungen lassen sich häufig wiederherstellen;
    beide sind danach aber nur in 3.209/15.194 Detailkopien korrekt.
11. **K:** Lokale ereignisgesteuerte Membran-Recovery ist biologisch und
    technisch plausibel, aber ihre geeignete Regel ist noch nicht bewiesen.
12. **L:** Vor einer Freigabe sind Recoveryzeitpunkt, Relaxationsstärke,
    Reizreihenfolge, zwei Beziehungen, Retention und Umweltumkehr gepaart
    gegen R0 zu prüfen.

## H. Abschlussentscheidung

B.5.3 ist als Kausalanalyse erfolgreich. K11/K12 werden nicht implementiert.
B.5.4 darf die bereits vorhandenen lokalen Recoverymodi als abgegrenzte
Forschungsvarianten prüfen. Ein globaler Reset, die Wiederherstellung eines
früher richtigen Zustands oder eine vorgegebene Motorentscheidung bleiben
unzulässig.
