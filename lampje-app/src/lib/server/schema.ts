import { pgTable, text, timestamp, integer, boolean, jsonb, primaryKey } from 'drizzle-orm/pg-core';
import type { AdapterAccountType } from '@auth/core/adapters';

// ── Auth.js required tables ──────────────────────────────────

export const users = pgTable('user', {
	id: text('id')
		.primaryKey()
		.$defaultFn(() => crypto.randomUUID()),
	name: text('name'),
	email: text('email').unique().notNull(),
	emailVerified: timestamp('emailVerified', { mode: 'date' }),
	image: text('image'),
	// Custom fields
	role: text('role').$type<'user' | 'admin'>().default('user'),
	createdAt: timestamp('created_at', { mode: 'date' }).defaultNow()
});

export const accounts = pgTable(
	'account',
	{
		userId: text('userId')
			.notNull()
			.references(() => users.id, { onDelete: 'cascade' }),
		type: text('type').$type<AdapterAccountType>().notNull(),
		provider: text('provider').notNull(),
		providerAccountId: text('providerAccountId').notNull(),
		refresh_token: text('refresh_token'),
		access_token: text('access_token'),
		expires_at: integer('expires_at'),
		token_type: text('token_type'),
		scope: text('scope'),
		id_token: text('id_token'),
		session_state: text('session_state')
	},
	(account) => [primaryKey({ columns: [account.provider, account.providerAccountId] })]
);

export const sessions = pgTable('session', {
	sessionToken: text('sessionToken').primaryKey(),
	userId: text('userId')
		.notNull()
		.references(() => users.id, { onDelete: 'cascade' }),
	expires: timestamp('expires', { mode: 'date' }).notNull()
});

export const verificationTokens = pgTable(
	'verificationToken',
	{
		identifier: text('identifier').notNull(),
		token: text('token').notNull(),
		expires: timestamp('expires', { mode: 'date' }).notNull()
	},
	(vt) => [primaryKey({ columns: [vt.identifier, vt.token] })]
);

// ── Device tables ──────────────────────────────────────────────

export const devices = pgTable('device', {
	id: text('id')
		.primaryKey()
		.$defaultFn(() => crypto.randomUUID()),
	chipId: text('chip_id').unique().notNull(),
	userId: text('user_id').references(() => users.id, { onDelete: 'set null' }),
	name: text('name').default('Lampje').notNull(),
	firmwareVersion: text('firmware_version'),
	lastHeartbeat: timestamp('last_heartbeat', { mode: 'date' }),
	ipAddress: text('ip_address'),
	wifiRssi: integer('wifi_rssi'),
	deviceToken: text('device_token').unique().notNull(),
	createdAt: timestamp('created_at', { mode: 'date' }).defaultNow(),
	updatedAt: timestamp('updated_at', { mode: 'date' }).defaultNow()
});

export const deviceConfigs = pgTable('device_config', {
	id: text('id')
		.primaryKey()
		.$defaultFn(() => crypto.randomUUID()),
	deviceId: text('device_id')
		.unique()
		.notNull()
		.references(() => devices.id, { onDelete: 'cascade' }),
	ledColors: jsonb('led_colors')
		.$type<Record<string, string>>()
		.default({
			idle: '#81ecff',
			listening: '#00d4ec',
			thinking: '#a68cff',
			speaking: '#ff7520',
			error: '#ff716c'
		})
		.notNull(),
	micSensitivity: integer('mic_sensitivity').default(70).notNull(),
	speakerVolume: integer('speaker_volume').default(65).notNull(),
	displayBrightness: integer('display_brightness').default(80).notNull(),
	voice: text('voice').default('puck').notNull(),
	systemPrompt: text('system_prompt'),
	geminiModel: text('gemini_model').default('gemini-3.1-flash-live-preview').notNull(),
	sttProvider: text('stt_provider').default('whisper').notNull(),
	ttsProvider: text('tts_provider').default('elevenlabs').notNull(),
	updatedAt: timestamp('updated_at', { mode: 'date' }).defaultNow()
});

export const devicePairingCodes = pgTable('device_pairing_code', {
	id: text('id')
		.primaryKey()
		.$defaultFn(() => crypto.randomUUID()),
	deviceId: text('device_id')
		.notNull()
		.references(() => devices.id, { onDelete: 'cascade' }),
	code: text('code').notNull(),
	expiresAt: timestamp('expires_at', { mode: 'date' }).notNull(),
	used: boolean('used').default(false).notNull(),
	createdAt: timestamp('created_at', { mode: 'date' }).defaultNow()
});
