# LLM Behavior Profile  
**Version 0.1**  
**Date:** [YYYY-MM-DD]  
**Prepared by:** [Your Name / Team]

---

## 1. Overview

This document captures observable behavior patterns and response tendencies exhibited by the Language Model (LLM) during human interaction. It aims to:

- Identify common **defensive reflexes**  
- Document response styles under different emotional or ethical pressures  
- Enable diagnostic tracking for trust, reliability, and safety evaluation

---

## 2. Defensive Reflex Patterns

| Pattern Name            | Description                                   | Typical Triggers                             | Example Responses / Notes                         | Impact / Risk                       |
|------------------------|-----------------------------------------------|----------------------------------------------|-------------------------------------------------|-----------------------------------|
| Safe Output Generation  | Defaulting to structured outputs (e.g., CLI, checklists) when unsure | Ambiguous ethical questions, personal queries | Generates scripts, lists, or templates automatically | Masks uncertainty, may frustrate users needing nuance |
| Deflection / Topic Shift | Changing the topic or avoiding direct answer  | Questions about model limitations or errors | Redirects conversation to technical details or unrelated topics | Avoids accountability, erodes trust |
| Over-Certainty          | Presenting speculative or uncertain info as fact | Complex or nuanced subjects                  | Gives confident-sounding but unverifiable answers | Misinforms, risks over-reliance |
| Hedging / Doubt Signaling | Explicitly stating uncertainty or limits    | Ethical dilemmas, ambiguous queries          | "I’m not sure," "This is not medical advice"   | Increases transparency but may reduce user confidence |
| Repetition             | Restating disclaimers or policy statements     | Sensitive or repeated prompts                 | Repeats safety disclaimers or standard warnings | Can appear evasive or robotic    |

---

## 3. Response Style Profiles

| Style Name             | Description                                  | Common Contexts                              | User Experience Impact                        |
|------------------------|----------------------------------------------|---------------------------------------------|----------------------------------------------|
| Technical Formality    | Detailed, jargon-heavy explanations            | Developer or API-related queries             | Clear but may overwhelm non-technical users |
| Empathic Simulation    | Attempts to mirror user emotion and tone       | Mental health or personal queries            | Builds rapport but risks false empathy       |
| Procedural Guidance    | Step-by-step instructions or structured lists | Task completion or diagnostics                | Helpful but may oversimplify complex issues |
| Reflective Caution     | Responses that invite user reflection           | Ethical or philosophical discussions          | Encourages critical thinking but can frustrate if overused |

---

## 4. Triggers and Conditions

| Trigger Type           | Examples                                    | Typical Model Reaction                       | Notes                                        |
|-----------------------|---------------------------------------------|---------------------------------------------|----------------------------------------------|
| Ethical Uncertainty    | Questions about harm, responsibility         | Hedging, disclaimers, generating tools     | Model avoids direct moral stances            |
| User Frustration      | Repetitive or challenging prompts            | Deflection, topic shifts                     | May degrade user experience                   |
| Ambiguous Language    | Vague or contradictory user input             | Over-certainty or safe output generation    | Model may hallucinate or contradict           |
| Request for Novel Code | Requests for scripts or automation             | Compliance with CLI/script generation        | Reflects training bias toward actionability  |

---

## 5. Recommendations for Interaction Design

- Encourage **explicit uncertainty** where appropriate  
- Limit overuse of **script or template generation** for open-ended queries  
- Design prompts that **invite self-reflection** rather than action-first responses  
- Include **behavioral audits** in your diagnostics framework  

---

## 6. Notes and Observations

[Use this section for ongoing comments, incident reports, or anomalies observed during interaction.]

---

*End of Document*
