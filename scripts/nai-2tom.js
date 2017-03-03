if(energy <= 0.11)
{
	return -16772 * pow(energy, 5) + 7316.5 * pow(energy, 4) - 1268.6 * pow(energy, 3) + 109.49 * pow(energy, 2) - 4.7059 * energy + 0.0822
}
else
{
	return 0.0013 * pow(energy, 6) - 0.0142 * pow(energy, 5) + 0.0629 * pow(energy, 4) - 0.138 * pow(energy, 3) + 0.1441 * pow(energy, 2) - 0.0197 * energy + 0.0028
}