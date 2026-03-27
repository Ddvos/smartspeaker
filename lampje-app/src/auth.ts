import { SvelteKitAuth } from '@auth/sveltekit';
import Google from '@auth/sveltekit/providers/google';
import Apple from '@auth/sveltekit/providers/apple';
import { DrizzleAdapter } from '@auth/drizzle-adapter';
import { db } from '$lib/server/db';
import * as schema from '$lib/server/schema';

export const { handle, signIn, signOut } = SvelteKitAuth({
	adapter: DrizzleAdapter(db, {
		usersTable: schema.users,
		accountsTable: schema.accounts,
		sessionsTable: schema.sessions,
		verificationTokensTable: schema.verificationTokens
	}),
	providers: [Google, Apple],
	pages: {
		signIn: '/login'
	},
	callbacks: {
		session({ session, user }) {
			session.user.id = user.id;
			return session;
		}
	},
	trustHost: true
});
