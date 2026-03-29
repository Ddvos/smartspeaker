import { db } from './db';
import { devices } from './schema';
import { eq } from 'drizzle-orm';

export function generateDeviceToken(): string {
	const bytes = new Uint8Array(32);
	crypto.getRandomValues(bytes);
	return Array.from(bytes)
		.map((b) => b.toString(16).padStart(2, '0'))
		.join('');
}

export function generatePairingCode(): string {
	const code = Math.floor(100000 + Math.random() * 900000);
	return code.toString();
}

export async function authenticateDevice(request: Request) {
	const authHeader = request.headers.get('authorization');
	if (!authHeader?.startsWith('Bearer ')) {
		return null;
	}

	const token = authHeader.slice(7);
	const [device] = await db.select().from(devices).where(eq(devices.deviceToken, token)).limit(1);

	return device ?? null;
}
