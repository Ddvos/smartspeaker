interface ToastItem {
	id: string;
	message: string;
	type: 'success' | 'error' | 'info';
}

let toasts = $state<ToastItem[]>([]);
let nextId = 0;

export const toastStore = {
	get items() {
		return toasts;
	},

	add(message: string, type: ToastItem['type'] = 'info') {
		const id = String(nextId++);
		toasts = [...toasts, { id, message, type }];
		setTimeout(() => {
			toasts = toasts.filter((t) => t.id !== id);
		}, 4000);
		return id;
	},

	dismiss(id: string) {
		toasts = toasts.filter((t) => t.id !== id);
	},

	success(message: string) {
		return this.add(message, 'success');
	},
	error(message: string) {
		return this.add(message, 'error');
	},
	info(message: string) {
		return this.add(message, 'info');
	}
};
