for (int submask = mask; ; submask = (submask - 1) & mask) { 
	// use submask
	if (submask == 0) break;
}

for (int upmask = mask; ; upmask = (upmask + 1) | mask) {
	// use upmask
	if (upmask == maxmask) break;
}