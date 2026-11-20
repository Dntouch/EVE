# Technische Analyse – subthreshold-Membrandynamik B.5.5

## 1. Kausale Ausgangslage

Die B.5.5-Inventur umfasst 768 Netze, getrennte Entdeckungs- und
Bestätigungsseeds, drei Stromfamilien sowie normale und inverse Zuordnungen.
In 393.216 natürlichen Episoden wurden 251.391 unterschwellige Zustände
beobachtet, entsprechend 0,639 Zuständen je Episode. Wegen Feuergrenze 2 und
ganzzahliger, nichtnegativer Membran hatten alle den Wert 1. Davon lagen
250.339 auf Entscheidungsneuronen und 1.052 auf Motorneuronen.

Vollständige Zustandskopien entfernten jeweils nur die konkrete lokale
Entscheidungsmembran. 62.172 Restpotenziale waren störend: Die natürliche
Antwort war falsch und wurde nach Entfernung korrekt. 7.417 waren nützlich:
Ihre Entfernung verhinderte eine korrekte Antwort. 169 änderten die Antwort,
ohne sie zu korrigieren; 180.581 blieben ohne beobachtbaren Folgeeffekt. Unter
84.679 natürlichen Fehlern waren 62.172, also 73,42 %, durch das isolierte
Restpotenzial korrigierbar. Die unabhängige Bestätigung ergibt je Cluster
73,57 % mit 95-%-KI 73,18 bis 73,94 %.

Alle 250.339 umgekehrten Wiederherstellungen reproduzierten den natürlichen
Zustand exakt. Der Kausalnachweis war damit vor Implementierung einer neuen
Regel positiv, zeigt aber zugleich, dass pauschales sofortiges Löschen falsch
wäre: 2,96 % der Entscheidungszustände waren in der Bestätigung unmittelbar
nützlich.

## 2. Implementierte Forschungssemantik

Die optionale Konfiguration `subthreshold_relaxation_enabled` plant beim
ersten Zustand `0 < membrane < threshold` genau ein lokales Ereignis. Der
Zeitpunkt ist `current_time + subthreshold_relaxation_delay`. Weitere
Eingänge, die dasselbe Neuron unterschwellig lassen, verschieben diesen
Zeitpunkt nicht. Die Semantik ist damit eine feste lokale Lebensdauer, kein
Inaktivitätstimer.

Ein gültiges Ereignis setzt ausschließlich die lokale Membran auf null. Es
kennt weder Reiz, Zielhandlung, Erfolg noch synaptische Gewichte. Feuern,
hemmende Rückkehr auf null, bestehende gültige R2-Recovery oder ein anderer
nicht-unterschwelliger Zustand invalidieren die Generation. Das alte Ereignis
bleibt deterministisch in der Queue und wird später als `stale` gezählt. Pro
Neuron kann höchstens ein Relaxationsereignis gleichzeitig gültig sein.

Die Funktion ist standardmäßig deaktiviert. Im deaktivierten Zustand werden
weder Ereignisse noch Generationen erzeugt. S5 ist in allen gepaarten
Naturverläufen exakt gleich S2; historische Ereignisfingerabdrücke bleiben
bei Nullwerten unverändert.

## 3. Ereignisreihenfolge

Die Queue bleibt nach Zeitstempel und Einfügesequenz geordnet. Bei
Batchverarbeitung werden alle bereits fälligen Nicht-Impuls-Ereignisse eines
Zeitpunkts in dieser Reihenfolge ausgeführt; gewöhnliche Impulse desselben
Zeitpunkts werden danach je Empfänger kanonisch gebündelt. Damit gilt:

- eine bereits fällige Subthreshold-Relaxation wirkt vor einem neuen
  gewöhnlichen Eingang desselben Zeitpunkts;
- Recovery, Eligibility-Ablauf, Feuer und Relaxation bleiben untereinander
  durch ihre Einfügesequenz deterministisch;
- von einem Feuer neu erzeugte Impulse werden in der folgenden Welle
  verarbeitet;
- Recovery und Subthreshold-Relaxation besitzen getrennte Generationen und
  getrennte Metriken.

Die Mikrotests decken gültige Fälligkeit, erneute erregende Eingänge, Feuer,
Hemmung, R2-Interaktion, veraltete Generationen, identische Zeitpunkte und
unabhängige Neuronen ab.

## 4. Zeitliche Integration

AA, BB, AB, BA, ABA und BAB wurden für Abstände 0, 1, 2, 4, 8, 16, 32 und 64
gegen Verzögerungen 1, 4, 8, 16, 32 und 64 geprüft. Von 288 Kombinationen
bewahrten 156 die natürliche Integration; in genau den 132 Fällen, in denen
die Relaxation vor dem nächsten relevanten Eingang fällig wurde, ging die
Integration verloren. Die Regel erzeugt damit eine klare zeitliche Grenze,
keine semantische Unterscheidung zwischen nützlicher und störender Aktivität.

Delay 16 wurde vor den großen Naturversuchen als zentrale Variante gewählt,
weil eine natürliche Episode 16 Zeiteinheiten umfasst: Integration innerhalb
dieses Horizonts bleibt erhalten, ältere Aktivität wird vor der folgenden
Episode begrenzt. Delay 32 und 64 waren vorab festgelegte Kontrollen. Sie
lieferten keinen positiven E1-Beitrag gegenüber S2; Delay 16 dagegen schon.

## 5. Persistenz und Ressourcen

Das Dateiformat wurde auf Version 14 erweitert. Konfiguration, zwei lokale
64-Bit-Zustände je Neuron, Queueereignisse und drei neue Metriken werden
gespeichert. Ältere Versionen laden die neue Funktion deaktiviert. Alle 64
Persistenzfälle stimmen vor Fälligkeit, nach Fälligkeit und nach identischer
Fortsetzung exakt überein.

In der unabhängigen gemischten Bestätigung plant S4-d16 0,671 Ereignisse je
Episode, führt 0,523 gültig aus und verwirft 0,147 als veraltet. Die Zahl aller
verarbeiteten Ereignisse steigt von 37,009 auf 37,433 je Episode. Die maximale
Queue bleibt bei 28 und wächst damit gegenüber S2 nicht. Zwei lokale
64-Bit-Felder benötigen strukturell 16 Byte je Neuron; der dynamisch
geschätzte Runtime-Speicher steigt im zentralen Lauf von 25.032 auf 25.072
Byte. Es gibt keinen globalen Tick, keine globale Membranliste und keine
unbegrenzte Ansammlung gültiger Ereignisse.

## 6. Technische Schlussfolgerung

Die feste lokale Lebensdauer ist als Forschungsmechanismus kausal begründet,
vollständig lokal, persistent, deterministisch und ressourcenbegrenzt. Sie
liefert einen eigenen positiven Beitrag gegenüber R2-50. Die strenge
Robustheitsvorgabe wird dennoch nicht erreicht. Daher bleibt die Funktion
abschaltbar und standardmäßig aus; B.5.5 belegt technische Eignung für weitere
Forschung, nicht die Freigabe als Produktionsstandard.

