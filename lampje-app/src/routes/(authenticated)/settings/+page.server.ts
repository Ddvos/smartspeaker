import type { PageServerLoad } from './$types';
import { db } from '$lib/server/db';
import { userSettings } from '$lib/server/schema';
import { eq } from 'drizzle-orm';

function maskApiKey(key: string): string {
	if (key.length <= 8) return '••••••••';
	return key.slice(0, 4) + '••••' + key.slice(-4);
}

export const load: PageServerLoad = async ({ locals }) => {
	const session = await locals.auth();
	if (!session?.user?.id) {
		return { settings: null };
	}

	let [settings] = await db
		.select()
		.from(userSettings)
		.where(eq(userSettings.userId, session.user.id))
		.limit(1);

	if (!settings) {
		[settings] = await db
			.insert(userSettings)
			.values({ userId: session.user.id })
			.returning();
	}

	return {
		settings: {
			hasGeminiApiKey: !!settings.geminiApiKey,
			geminiApiKeyMasked: settings.geminiApiKey ? maskApiKey(settings.geminiApiKey) : '',
			geminiApiKeyStatus: settings.geminiApiKeyStatus,
			defaultModel: settings.defaultModel,
			defaultVoice: settings.defaultVoice,
			defaultSystemPrompt: settings.defaultSystemPrompt ?? ''
		}
	};
};
