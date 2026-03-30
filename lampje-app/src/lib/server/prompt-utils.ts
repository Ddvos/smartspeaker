const DEFAULT_SYSTEM_PROMPT =
	'Je bent Lampje, een vriendelijke en behulpzame slimme lamp. Je spreekt Nederlands en helpt de gebruiker met vragen, taken en gezellig kletsen. Houd je antwoorden kort en natuurlijk.';

export function getSystemPrompt(customPrompt: string | null | undefined): string {
	return customPrompt?.trim() || DEFAULT_SYSTEM_PROMPT;
}
