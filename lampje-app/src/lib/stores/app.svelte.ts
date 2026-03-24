let sidebarCollapsed = $state(false);
let commandPaletteOpen = $state(false);

export const appStore = {
	get sidebarCollapsed() {
		return sidebarCollapsed;
	},
	toggleSidebar() {
		sidebarCollapsed = !sidebarCollapsed;
	},
	get commandPaletteOpen() {
		return commandPaletteOpen;
	},
	toggleCommandPalette() {
		commandPaletteOpen = !commandPaletteOpen;
	}
};
