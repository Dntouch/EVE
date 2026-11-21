# Forschungsbericht – Forschungsphase C.1

## Methodisch korrigierter Abschluss

**Datum:** 21. November 2026

**Status:** nachträgliche methodische Korrektur abgeschlossen; keine neue
Produktions-Lernregel freigegeben

## Korrekturhinweis

Der ursprüngliche C.1-Lauf setzte W0 und W1 erst bei der Gewichtsschreibung
nach der Motorhandlung auseinander. Aus ihrer identischen Handlung durfte
daher keine Neutralität der beteiligten Synapse abgeleitet werden. Ebenso
belegten andere starke Eingänge und einzelne Kontrollpunkte weder eine
funktionale Reorganisation noch kontinuierliche Funktionslosigkeit.

Die ursprünglichen Daten unter `research/results/c1` bleiben unverändert. Die
post-hoc ergänzten H0–H3-Interventionen, Ablationen alternativer Eingänge und
Schritt-für-Schritt-Diagnosen liegen getrennt unter
`research/results/c1/method-correction`.

Die nochmals post-hoc ergänzten Proben K1 und K2 liegen ausschließlich unter
`research/results/c1/method-correction/additional-probes`. Sie korrigieren die
Interpretation der partiellen Abschwächung und der alternativen Pfade, ohne
frühere Rohdaten zu verändern.

## Korrigierter Kurzbefund

- Alle 38 ausgewählten Synapsen waren an der fehlerhaften Motorentscheidung
  kausal beteiligt: H0 erzeugte 38/38-mal eine falsche Doppelantwort, H1 mit
  vor der Erfahrung deaktivierter Synapse 38/38-mal die korrekte Einzelantwort.
  H2 reproduzierte H0 in 38/38 Fällen exakt.
- Die vollständige H1-Ablation ist in 38/38 Fällen nützlich. K1 zeigt jedoch,
  dass Gewicht 1 wie Gewicht 2 weiterhin 38/38 falsche Doppelantworten erzeugt.
  Die tatsächlich untersuchte 2→1-Abschwächung ist daher situativ 38-mal
  neutral; nur Gewicht 0 verhindert den Fehler.
- Die unmittelbare direkte Zielwirkung der ausgewählten Synapse ist in 38/38
  Fällen kausal nachgewiesen.
- Jeder Fall besitzt kontrolliert aktivierbare und fähigkeitsrelevante
  Alternativen. K2 zeigt aber, dass diese Unterstützung bereits vor dem
  kritischen Verlust vorhanden ist: 499/499 verfolgte Alternativen existieren
  vorher, 76 sind bereits kontrolliert funktional und 38 ersetzen den
  ursprünglichen Pfad bereits in der Vorverlustprobe. Kein alternativer Pfad
  wird erstmals später funktional. Ein zeitlicher Funktionsübergang ist damit
  nicht belegt.
- Die ursprüngliche direkte Funktion kehrt in 38/38 Fällen zwischenzeitlich
  zurück: erstmals zwischen Erfahrung 10 und 47, insgesamt an 388
  diagnostizierten Schritten nach dem ersten Verlust. Es treten 356 erneute
  Verluste auf. Am Schritt 256 sind weiterhin 0/38 Pfade direkt funktional.
- Die W0/W1-Langzeitklassen bleiben unverändert: 16 nützlich, zehn schädlich,
  sieben neutral und fünf kontextabhängig. Sie beschreiben Unterschiede
  vollständiger Lerntrajektorien, nicht die Güte einer einzelnen Synapse.

## Umfang und Reproduktion

| Kennzahl | Ergebnis |
|---|---:|
| unabhängiger Sicherheits-Preflight | 12/12 |
| Regressionstests | 834/834 |
| B.5.11 / B.5.12 / B.5.13 Referenzdateien | 21 / 15 / 13 exakt |
| C.0 Referenzdateien | 14 exakt |
| historische C.0-Funktionsfälle | 19/19 |
| prospektive Läufe | 12.800 |
| registrierte neue Verbindungen | 166.880 |
| funktionale prospektive Fälle | 19 |
| Discovery / unabhängige Bestätigung | 7 / 12 |
| ausgewählte Fälle insgesamt | 38 |
| exakte Wiederholung aller korrigierten C.1-Shards | bestanden |
| exakte Wiederholung aller K1/K2-Shards | bestanden |
| Persistenz mit ausstehenden Ereignissen | bestanden |

## Vier getrennte Dimensionen

### A – Integrität

Der unabhängige Preflight bestand vor allen Lernexperimenten. Alle
Veränderungen waren nach Architekturentscheidung 0037 zulässig. Keine
Funktions- oder Erfolgsbeobachtung wurde zur Legitimierung eines
Integritätsverstoßes verwendet.

### B – Kausalität

H0, H1 und H2 starten aus demselben Zustand. H1 setzt nur die ausgewählte
Synapse vor Beginn der natürlichen Erfahrung diagnostisch auf null. Die
abweichende Handlung erhält ihre tatsächlich entstandene positive
Rückmeldung. H3 entfernt kombiniert die konkurrierenden Eingänge. Erfasst
werden Motorantwort, frühe und doppelte Aktivierung, Korrektheit, Konsequenz,
Zielaktivität sowie weitere Neuronen- und Synapsenaktivität.

Der Handlungsbeitrag ist eindeutig: Die ausgewählte Synapse verursacht in
38/38 Fällen den Übergang von der korrekten Einzelantwort zur falschen
Doppelantwort. Davon getrennt zeigt W0/W1, dass die spätere 2→1-Schreibung in
0/38 Fällen die nächste Entscheidung verändert. Die direkte Senderprobe zeigt
in 38/38 Fällen die unmittelbare Zielwirkung.

K1 setzt das Gewicht vor derselben Erfahrung auf 2, 1 oder 0; K1.3 ist die
Identitätskontrolle. Gewicht 2 und 1 ergeben jeweils 38 falsche
Doppelantworten, Gewicht 0 ergibt 38 korrekte Einzelantworten. Nach K1.0 und
K1.1 ist die nächste Entscheidung jeweils 38-mal korrekt; nach K1.2 dagegen
38-mal falsch, weil die abweichende korrekte Handlung positive Schriften
auslöst. Partielle Abschwächung und vollständige Ablation sind kausal nicht
gleichwertig.

### C – Situative Angemessenheit

Die tatsächliche 2→1-Abschwächung ist 38-mal neutral: Vor der Erfahrung auf
Gewicht 1 gesetzt, verändert sie weder Handlung noch Korrektheit gegenüber
Gewicht 2. Die vollständige Deaktivierung auf Gewicht 0 ist 38-mal nützlich,
darf aber nicht stellvertretend für die natürliche Teilabschwächung stehen.

### D – Langzeitwirkung

Die ursprünglichen Kategorien bleiben bei unveränderten Schwellenwerten
bestehen:

| Population | nützlich | schädlich | neutral | kontextabhängig |
|---|---:|---:|---:|---:|
| historisch | 7 | 4 | 3 | 5 |
| prospektiv | 9 | 6 | 4 | 0 |
| gesamt | 16 | 10 | 7 | 5 |

Jede Zeile nennt nun die geprüfte Fähigkeit (Abruf von Aufgabe A und B), den
kausalen Unterschied, den Maßstab (korrekte Erfahrungen und/oder finales
Repertoire), die nachgewiesene Rolle alternativer Pfade und die Abhängigkeit
von auseinanderlaufenden Lerntrajektorien. Alle 38 Vergleiche sind
trajektorienabhängig.

## Alternative Pfade und Wiederkehr

Pro ausgewähltem Pfad werden strukturelle, potenziell funktionale,
kontrolliert funktionale, neuronal kausale und fähigkeitskausale Alternativen
getrennt. In jedem Fall bestehen zwei kontrolliert aktivierbare Alternativen;
eine davon ist in der Abrufprobe fähigkeitskausal. K2 verfolgt 499 alternative
Eingänge mit K2.0–K2.4 vor und nach dem Verlust, an allen festen Horizonten und
an jedem Verlust-/Wiederkehrereignis. Alle 499 Verbindungen bestehen bereits
vor dem kritischen Verlust. Die 76 jemals kontrolliert funktionalen
Alternativen sind ausnahmslos schon an der Vorverlustprobe funktional; 423
werden nie kontrolliert funktional. Alle 38 ersetzenden Pfade unterstützen den
Abruf bereits vor dem Verlust. Die Daten belegen daher vorbestehende verteilte
Unterstützung, aber keinen späteren Funktionsübergang und keine funktionale
Reorganisation im zeitlichen Sinn.

Die direkte Funktion wurde in diagnostischen Zustandskopien nach jedem Schritt
0–256 geprüft. Alle 38 Pfade verlieren und gewinnen Funktion mehrfach; kein
Fall ist am letzten Schritt funktional. Aussagen über kontinuierliche
Funktionslosigkeit wurden entfernt.

## Antworten auf die Abschlussfragen

1. **Handlungskausalität:** Ja, 38/38 Synapsen waren kausal an der falschen
   Doppelantwort beteiligt.
2. **Situative 2→1-Abschwächung:** 0 nützlich, 0 schädlich, 38 neutral,
   0 unentschieden. Nur die 2→0-Ablation ist situativ nützlich.
3. **Funktionale Alternativen:** Ja, kontrollierte Zielaktivierung in 38/38
   Fällen.
4. **Zeitlicher Funktionsübergang:** Nein. Die 38 ersetzenden Pfade sind schon
   vor dem Verlust funktional und fähigkeitsrelevant.
5. **Zwischenzeitliche Wiederkehr:** Ja, 38/38, erstmals bei Schritt 10–47;
   danach wiederholte Verluste, 0/38 bei Schritt 256.
6. **Langzeitklassen:** Ja, 16/10/7/5 bleiben gültig, aber nur als
   trajektorienabhängige W0/W1-Vergleiche.
7. **Für C.2 abgesichert:** Eligibility und lokale Modulation erklären die
   Schreibung; partielle Abschwächung, vollständige Ablation, bestehende
   Redundanz und zeitlicher Übergang müssen getrennt bleiben.

## Merge- und Produktionsentscheidung

**C.1 nach `main` mergen: Ja – für Forschungsinfrastruktur, korrigierte
Methodik und wissenschaftliche Befunde.**

**Produktionsfreigabe: Nein.** Es wird keine neue Lernregel, kein pauschaler
Gewichtsschutz, kein Einfrieren, kein Rollback und keine Aktivierung von
Growth, interner Exploration oder S4-d16 freigegeben.
