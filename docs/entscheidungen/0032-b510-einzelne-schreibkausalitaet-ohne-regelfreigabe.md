# 0032 – B.5.10: Einzelne Schreibkausalität ohne Regelfreigabe

**Status:** angenommen  
**Datum:** 20. November 2026

## Entscheidung

Die standardmäßig inaktive Schnittstelle zur isolierten Unterdrückung einer
konkreten synaptischen Schreiboperation wird als Forschungsdiagnostik
integriert. Es wird keine automatische lokale Schreibauswahl, kein Rollback,
kein Stabilitäts-Gate und keine neue persistente Synapsenhistorie eingeführt.

## Begründung

Die Intervention ist technisch isoliert und reproduzierbar. Historisch sind
13 konkrete positive Schritte auf zehn Problemverbindungen kausal; kombiniert
werden 12/12 Verlustfälle verhindert. Prospektiv waren jedoch 21/119 positive
Schritte unmittelbar notwendig und 98/119 zunächst neutral. Elf neutrale
Schritte wurden später schädlich, drei später nützlich.

Die aktuelle lokale Signatur zeigt ein Klassifikationssignal, wurde aber nur
an 59 Bestätigungsereignissen geprüft. Extern rekonstruierte Historien
generalisieren schlechter und sind keine lokal verfügbare Information. Eine
Regel wäre daher wissenschaftlich und architektonisch verfrüht.

## Folgen

- Der Interceptor bleibt leer, nicht persistent und außerhalb der neuronalen
  Auswahlentscheidung.
- Persistenzformat 15 bleibt unverändert.
- Zusätzlicher persistenter Speicher je Neuron und Synapse bleibt null.
- S4 und die globale Schreibsperre bleiben standardmäßig deaktiviert.
- K23/K24 werden nicht umgesetzt.
- B.5.11 muss eine präregistrierte A/B-Hypothese kausal auf neuen Ereignissen
  prüfen, bevor eine Forschungsregel erwogen wird.
