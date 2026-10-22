from PIL import Image
import matplotlib.pyplot as plt
from pathlib import Path

# Digite o caminho completo da imagem
caminho = input("Digite o caminho completo da imagem: ").strip().strip('"')

# Verifica se a imagem existe
if not Path(caminho).is_file():
    print("Erro: imagem não encontrada.")
    exit()

# Abre a imagem
imagem = Image.open(caminho).convert("RGB")

# Obtém os canais RGB
r, g, b = imagem.split()

# Gera o histograma
plt.figure(figsize=(10, 6))

plt.hist(
    list(r.getdata()),
    bins=256,
    range=(0, 256),
    color="red",
    alpha=0.5,
    label="Vermelho (R)"
)

plt.hist(
    list(g.getdata()),
    bins=256,
    range=(0, 256),
    color="green",
    alpha=0.5,
    label="Verde (G)"
)

plt.hist(
    list(b.getdata()),
    bins=256,
    range=(0, 256),
    color="blue",
    alpha=0.5,
    label="Azul (B)"
)

plt.title("Histograma da imagem")
plt.xlabel("Intensidade dos pixels (0–255)")
plt.ylabel("Frequência")
plt.legend()
plt.grid(alpha=0.2)

# Salva o histograma na mesma pasta da imagem
saida = Path(caminho).parent / f"{Path(caminho).stem}_histograma.png"

plt.savefig(saida, dpi=300, bbox_inches="tight")
plt.show()

print(f"\nHistograma salvo em: {saida}")