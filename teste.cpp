#include <gtkmm.h>
#include <glibmm.h>
#include <iostream>
#include <vector>

// 1. Uma estrutura C++ normal para guardar os dados
struct DadosItem {
    std::string texto;
};

// 2. Janela Principal
class JanelaPrincipal : public Gtk::Window {
private:
    Gtk::ScrolledWindow m_scrolled_window;
    Gtk::GridView m_grid_view;
    
    // Em vez de criar um objeto GObject customizado, usamos o ListStore de um tipo já existente, 
    // como Glib::BoxedWrapper<std::string> (ou simplesmente guardamos os textos)
    Glib::RefPtr<Gio::ListStore<Glib::ustring>> m_modelo;
    std::vector<DadosItem> m_lista_dados; // Sua lista real de dados C++

public:
    JanelaPrincipal() {
        set_title("Exemplo de GTKmm 4 GridView");
        set_default_size(400, 300);

        m_scrolled_window.set_child(m_grid_view);
        set_child(m_scrolled_window);

        // Criamos o modelo guardando strings simples (que o Glib já sabe envelopar perfeitamente)
        m_modelo = Gio::ListStore<Glib::ustring>::create();

        // Popula os dados
        for (int i = 1; i <= 24; ++i) {
            std::string texto_item = "Item " + std::to_string(i);
            m_lista_dados.push_back({texto_item});
            m_modelo->append(texto_item); // Adiciona a string ao modelo
        }

        auto fabrica = Gtk::SignalListItemFactory::create();
        
        // Passo A: Instancia o botão físico na tela
        fabrica->signal_setup().connect([](const Glib::RefPtr<Gtk::ListItem>& list_item) {
            auto botao = Gtk::make_managed<Gtk::Button>();
            botao->set_size_request(80, 80);
            list_item->set_child(*botao);
        });

        // Passo B: Amarra o texto da linha correspondente ao botão
        fabrica->signal_bind().connect([this](const Glib::RefPtr<Gtk::ListItem>& list_item) {
            // Pegamos a string que está guardada na posição atual da Grid
            auto ustring_ptr = std::dynamic_pointer_cast<Glib::BoxedWrapper<Glib::ustring>>(list_item->get_item());
            auto botao = dynamic_cast<Gtk::Button*>(list_item->get_child());

            if (ustring_ptr && botao) {
                Glib::ustring texto = ustring_ptr->get_object();
                botao->set_label(texto);
            }
        });

        auto selecao = Gtk::SingleSelection::create(m_modelo);
        m_grid_view.set_model(selecao);
        m_grid_view.set_factory(fabrica);
        
        m_grid_view.set_max_columns(4);
    }
};

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create("org.gtkmm.gridview.exemplo");
    return app->make_window_and_run<JanelaPrincipal>(argc, argv);
}